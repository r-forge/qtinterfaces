#include "SmokeObject.hpp"
#include "SmokeClass.hpp"
#include "RQtModule.hpp"
#include "InstanceObjectTable.hpp"

#include <Rinternals.h>

//#define MEM_DEBUG

/* One SmokeObject for each object,
   to ensure 1-1 mapping from Qt objects to R objects */
QHash<void *, SmokeObject *> SmokeObject::instances;

SmokeObject * SmokeObject::fromPtr(void *ptr, const Class *klass,
                                   bool allocated, bool copy)
{
  if (!klass)
    error("Attempt to create SmokeObject with NULL class");
  if (!ptr)
    error("Attempt to create SmokeObject with NULL pointer");
  SmokeObject *so = instances[ptr];
  if (!so) {
    so = new SmokeObject(ptr, klass, allocated);
#ifdef MEM_DEBUG
    qDebug("%p: created for %p (%s)", so, ptr, klass->name());
#endif
    // record this ASAP, resolveClassId() needs it for virtual callbacks
    instances[so->ptr()] = so; 
    so->cast(Class::fromSmokeId(so->smoke(), so->module()->resolveClassId(so)));
    /* it seems that all multiple inheritance in Qt goes through
       QObject or QEvent, so we can catch offset pointers at run-time */
    // FIXME: what happens with other libraries? take QtRuby approach?
    if (so->ptr() != ptr) { // must be multiple inheritance, recache
      SmokeObject *tmp_so = instances[so->ptr()];
#ifdef MEM_DEBUG
      qDebug("%p: multiple inheritance detected, switch to %p", so, so->ptr());
#endif
      if (tmp_so) {
#ifdef MEM_DEBUG
      qDebug("%p: replaced with existing %p", so, tmp_so);
#endif
        delete so;
        so = tmp_so;
        copy = false; // don't think we every want to copy here
      } else instances[so->ptr()] = so;
      instances.remove(ptr);
    }
    if (copy) { // copy the data
      void *tmp_ptr = so->ptr();
      so->_ptr = so->clonePtr(); 
      instances[so->ptr()] = so; // update the instances hash after cloning
      instances.remove(tmp_ptr);
      so->_allocated = true;
    }
  }
  return so;
}

SmokeObject *SmokeObject::fromPtr(void *ptr, Smoke *smoke, const char *name,
                                  bool allocated, bool copy)
{
  return fromPtr(ptr, Class::fromSmokeName(smoke, name), allocated, copy);
}

SmokeObject *SmokeObject::fromPtr(void *ptr, Smoke *smoke, int classId,
                                  bool allocated, bool copy)
{
  return fromPtr(ptr, Class::fromSmokeId(smoke, classId), allocated, copy);
}

SmokeObject *SmokeObject::fromPtr(void *ptr, const SmokeType &type,
                                  bool allocated, bool copy)
{
  return fromPtr(ptr, type.smoke(), type.classId(), allocated, copy);
}

SEXP
SmokeObject::sexpFromPtr(void *ptr, const Class *klass,
                         bool allocated, bool copy)
{
  SmokeObject *so = fromPtr(ptr, klass, allocated, copy);
  if (!so)
    return R_NilValue;
  else return so->sexp();
}

SEXP
SmokeObject::sexpFromPtr(void *ptr, Smoke *smoke, const char *name,
                         bool allocated, bool copy)
{
  return sexpFromPtr(ptr, Class::fromSmokeName(smoke, name), allocated, copy);
}

SEXP
SmokeObject::sexpFromPtr(void *ptr, Smoke *smoke, int classId,
                         bool allocated, bool copy)
{
  return sexpFromPtr(ptr, Class::fromSmokeId(smoke, classId), allocated, copy);
}

SEXP
SmokeObject::sexpFromPtr(void *ptr, const SmokeType &type,
                                      bool allocated, bool copy)
{
  return sexpFromPtr(ptr, type.smoke(), type.classId(), allocated, copy);
}

#define SMOKE_OBJECT_FROM_VALUE(klass, sexp) ({                   \
      klass val = as ## klass(sexp);                              \
      SmokeObject::fromPtr(&val, NULL, #klass, false, true);      \
})

SmokeObject * SmokeObject::fromSexp(SEXP sexp)
{
  if (!isEnvironment(sexp))
    error("Expected an environment");
  return InstanceObjectTable::instanceFromSexp(HASHTAB(sexp));
}

SmokeObject::SmokeObject(void *ptr, const Class *klass, bool allocated)
  : _ptr(ptr), _klass(klass), _allocated(allocated), _sexp(NULL),
    _internalTable(NULL), _fieldEnv(NULL)
{
}

void SmokeObject::maybeDestroy() {
  if (_allocated && !memoryIsOwned()) {
#ifdef MEM_DEBUG
    qDebug("%p: invoking destructor", this);
#endif
    const char *cname = _klass->smokeBase()->name();
    const char *colon = strrchr(cname, ':');
    if (colon)
      cname = colon + 1;
    char *destructor = new char[strlen(cname) + 2];
    destructor[0] = '~';
    strcpy(destructor + 1, cname);
    invokeMethod(destructor); // might cause 'this' to be destructed
    delete[] destructor;
    if (instances.contains(_ptr)) // we are still around
      delete this;
  }
}

void SmokeObject::invalidateSexp() {
#ifdef MEM_DEBUG
  qDebug("%p: invalidating sexp %p", this, _sexp);
#endif
  _sexp = NULL;
  maybeDestroy();
}

void SmokeObject::invalidateInternalTable() {
#ifdef MEM_DEBUG
  qDebug("%p: invalidating internal table %p", this, _internalTable);
#endif
  _internalTable = NULL;
  maybeDestroy();
}

void SmokeObject::castSexp(SEXP sexp) {
  SEXP rclasses;
  QList<const Class *> classes = _klass->ancestors();
  classes.prepend(_klass);
  rclasses = allocVector(STRSXP, classes.size() + 3);
  setAttrib(sexp, R_ClassSymbol, rclasses);
  for (int i = 0; i < classes.size(); i++)
    SET_STRING_ELT(rclasses, i, mkChar(classes[i]->name()));
  SET_STRING_ELT(rclasses, length(rclasses) - 3, mkChar("UserDefinedDatabase"));
  SET_STRING_ELT(rclasses, length(rclasses) - 2, mkChar("environment"));
  SET_STRING_ELT(rclasses, length(rclasses) - 1, mkChar("RQtObject"));
}
  
SEXP SmokeObject::createSexp(SEXP parentEnv) {
  SEXP env;
  PROTECT(env = allocSExp(ENVSXP));
  SET_ENCLOS(env, parentEnv);
  castSexp(env);  
  UNPROTECT(1);
  return env;
}

SEXP SmokeObject::sexp() {
  if (!_sexp) {
    _sexp = createSexp(R_EmptyEnv);
    SET_HASHTAB(_sexp, _klass->createObjectTable(this)->sexp());
#ifdef MEM_DEBUG
    qDebug("%p: created sexp %p", this, _sexp);
#endif
  }
  return _sexp;
}

SEXP SmokeObject::fieldEnv() const {
  if (!_fieldEnv) {
    _fieldEnv = allocSExp(ENVSXP);
    SET_ENCLOS(_fieldEnv, R_EmptyEnv);
    SET_FRAME(_fieldEnv, R_NilValue);
    R_PreserveObject(_fieldEnv);
  }
  return _fieldEnv;
}

Smoke *SmokeObject::smoke() const { return _klass->smokeBase()->smoke(); }
int SmokeObject::classId() const { return _klass->smokeBase()->classId(); }
const char *SmokeObject::className() const { return _klass->name(); }

RQtModule *SmokeObject::module() const {
  return RQtModule::module(smoke());
}

bool SmokeObject::memoryIsOwned() const {
  // NOTE: calling the module's memoryIsOwned() might resurrect sexps;
  // if so, they become orphans
  bool owned = _sexp || _internalTable;
  if (!owned) {
    owned = module()->memoryIsOwned(this);
#ifdef MEM_DEBUG
    if (owned)
      qDebug("%p: memory is owned by Qt", this);
#endif
  }
#ifdef MEM_DEBUG
  else qDebug("%p: memory is owned by R, sexp: %p, table: %p", this, _sexp,
                _internalTable);
#endif
  return owned;
}

// only works for pure Smoke instances, but that may be OK
void * SmokeObject::clonePtr() const {
  Smoke *smoke = this->smoke();
  const char *className = _klass->name();
  int classNameLen = strlen(className);

  // copy constructor signature
  QByteArray ccSig(className);
  int pos = ccSig.lastIndexOf("::");
  if (pos != -1) {
    ccSig = ccSig.mid(pos + strlen("::"));
  }
  ccSig.append("#");
  Smoke::ModuleIndex ccId = smoke->findMethodName(className, ccSig);

  char *ccArg = new char[classNameLen + 8];
  sprintf(ccArg, "const %s&", className);

  Smoke::ModuleIndex classIdx = Smoke::ModuleIndex(smoke, classId());
  Smoke::ModuleIndex ccMeth = smoke->findMethod(classIdx, ccId);

  if (ccMeth.index == 0) {
    qWarning("failed to construct copy: %s %p\n", className, _ptr);
    delete[] ccArg;
    return 0;
  }
  Smoke::Index method = ccMeth.smoke->methodMaps[ccMeth.index].method;
  if (method > 0) {
    delete[] ccArg;
    if (!(ccMeth.smoke->methods[method].flags & Smoke::mf_copyctor)) {
      qCritical("failed to construct copy: %s %p\n", className, _ptr);
      return 0;
    }
    ccMeth.index = method;
  } else {
    // ambiguous method, pick the copy constructor
    Smoke::Index i = -method;
    while (ccMeth.smoke->ambiguousMethodList[i]) {
      if (ccMeth.smoke->methods[ccMeth.smoke->ambiguousMethodList[i]].flags &
          Smoke::mf_copyctor)
        break;
      i++;
    }
    delete[] ccArg;
    ccMeth.index = ccMeth.smoke->ambiguousMethodList[i];
    if (ccMeth.index == 0) {
      qCritical("construct_copy() failed %s %p\n", className, _ptr);
      return 0;
    }
  }

  // Okay, ccMeth is the copy constructor. Time to call it.
  Smoke::StackItem args[2];
  args[0].s_voidp = 0;
  args[1].s_voidp = _ptr;
  Smoke::ClassFn fn = smoke->classes[classId()].classFn;
  (*fn)(smoke->methods[ccMeth.index].method, 0, args);

  // Initialize the binding for the new instance
  Smoke::StackItem s[2];
  s[1].s_voidp = module()->binding();
  (*fn)(0, args[0].s_voidp, s);

  return args[0].s_voidp;
}

SmokeObject *SmokeObject::clone() const {
  return SmokeObject::fromPtr(clonePtr(), _klass, _allocated);
}

void
SmokeObject::cast(const Class *klass) {
  _ptr = castPtr(klass->name());
  _klass = klass;
  if (_sexp)
    castSexp(_sexp);
}

/* Cast the instance pointer to a parent class. This is necessary,
   because the compiler does not know how to cast a void* to a parent
   class when multiple inheritance is involved.
*/
void *
SmokeObject::castPtr(const char *className) const {
  Smoke *smoke = this->smoke();
  // Be very careful changing this -- it is written as intended
  return smoke->cast(_ptr, classId(), smoke->idClass(className, true).index);
}

bool // result undefined if class names are not all unique
SmokeObject::instanceOf(const char *className) const {
  Smoke *smoke = this->smoke();
  // the base class must be 'found', i.e., it cannot be external
  Smoke::ModuleIndex other = smoke->findClass(className);
  return smoke->isDerivedFrom(smoke, classId(), other.smoke, other.index);
}

bool
SmokeObject::instanceOf(const SmokeType &type) const {
  Smoke *smoke = this->smoke();
  return smoke->isDerivedFrom(smoke, classId(), type.smoke(), type.classId());
}

void
SmokeObject::invokeMethod(const char *name, Smoke::Stack stack) {
  Smoke *smoke = this->smoke();
  Smoke::ModuleIndex nameId = smoke->idMethodName(name);
  Smoke::ModuleIndex classIdx = Smoke::ModuleIndex(smoke, classId());
  Smoke::ModuleIndex meth = nameId.smoke->findMethod(classIdx, nameId);
  if (meth.index > 0) {
    Smoke::Method &m =
      meth.smoke->methods[meth.smoke->methodMaps[meth.index].method];
    Smoke::ClassFn fn = meth.smoke->classes[m.classId].classFn;
    (*fn)(m.method, _ptr, stack);
  } else {
    error("Cannot find (unambiguous) %s::%s",
          smoke->classes[classIdx.index].className, name);
  }
}

/* Sometimes we want the env to provide all symbols, othertimes just
   the public symbols. Approaches:
   - Multiple environments: complicates memory management, as we rely
     on having one env per SmokeObject
   - Change 'inside' setting on the fly. Way too complicated.
   To handle the multiple environment case, all envs would need to
   'own' the instance. We track the references with a QMap.
*/

SEXP SmokeObject::internalSexp(SEXP env) {
  SEXP sexp;
  PROTECT(sexp = createSexp(env));
  SET_HASHTAB(sexp, internalTable());
  UNPROTECT(1);
  return sexp;
}

SEXP SmokeObject::internalTable() {
  if (!_internalTable) {
    InstanceObjectTable *table = _klass->createObjectTable(this);
    table->setInternal(true);
    _internalTable = table->sexp();
#ifdef MEM_DEBUG
    qDebug("%p: creating internal table %p", this, _internalTable);
#endif
  }
  return _internalTable;
}

SEXP SmokeObject::enclose(SEXP fun) {
  SEXP dupFun;
  PROTECT(dupFun = duplicate(fun));
  SET_CLOENV(dupFun, internalSexp(CLOENV(fun)));
  UNPROTECT(1);
  return dupFun;
}

void SmokeObject::orphanTable(SEXP sexp) const {
  InstanceObjectTable *table = 
    static_cast<InstanceObjectTable *>(ObjectTable::fromSexp(sexp));
  table->setInstance(NULL);
}

SmokeObject::~SmokeObject() {
#ifdef MEM_DEBUG
  qDebug("%p: destructing", this);
#endif
  if (_sexp) {
#ifdef MEM_DEBUG
    qDebug("%p: orphaned sexp %p", this, _sexp);
#endif
    orphanTable(HASHTAB(_sexp));
  }
  if (_internalTable) {
#ifdef MEM_DEBUG
    qDebug("%p: orphaned internal table %p", this, _internalTable);
#endif
    orphanTable(_internalTable);
  }
  if (_fieldEnv)
    R_ReleaseObject(_fieldEnv);
  instances.remove(_ptr);
}
