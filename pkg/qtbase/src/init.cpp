#include "convert.hpp"
#include "Reference.hpp"

#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Print.h>

void init_smoke();
void init_type_handlers();

extern "C" {
  
    // Test.cpp
    SEXP newLabelWidget(SEXP label);

    // style.cpp

    SEXP qt_qsetStyleSheet(SEXP x, SEXP s);
    SEXP qt_qstyleSheet(SEXP x);

    // this file 

    SEXP addQtEventHandler();
    SEXP cleanupQtApp();

    void R_init_qtbase(DllInfo *dll);

    extern void init_utils();

  // timer
  SEXP qt_qtimer(void);
  
  // signals
  SEXP qt_qconnect(SEXP x, SEXP user_data, SEXP handler, SEXP which);

  // metadata
  SEXP qt_qmethods(SEXP klass);
  SEXP qt_qenums(SEXP klass);
  SEXP qt_qproperties(SEXP x);
  SEXP qt_qclasses(SEXP rsmoke);

  // Smoke-specific metadata
  SEXP qt_qsmokes(void);

  // Moc-specific metadata
  SEXP qt_qnormalizedSignature(SEXP x);
  SEXP qt_qmocMethods(SEXP x);
  
  // dynamic invocation
  SEXP qt_qinvoke(SEXP method, SEXP self, SEXP args);
  SEXP qt_qinvokeStatic(SEXP method, SEXP smoke, SEXP klass, SEXP args);
  
  // properties
  SEXP qt_qsetProperty(SEXP x, SEXP rname, SEXP rvalue);
  SEXP qt_qproperty(SEXP x, SEXP name);

  // user classes
  SEXP qt_qcast(SEXP x, SEXP className);
  SEXP qt_qenclose(SEXP x, SEXP fun);
}

extern void initMethodSelectors();

QObject* unwrapQObjectReferee(SEXP x);
QGraphicsItem *unwrapQGraphicsItemReferee(SEXP x);

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static R_CallMethodDef CallEntries[] = {

    CALLDEF(newLabelWidget, 1),

    CALLDEF(qt_qsetStyleSheet, 2),
    CALLDEF(qt_qstyleSheet, 1),

    CALLDEF(addQtEventHandler, 0),
    CALLDEF(cleanupQtApp, 0),

    CALLDEF(qt_qtimer, 0),
    
    CALLDEF(qt_qconnect, 5),

    CALLDEF(qt_qmocMethods, 1),
    
    CALLDEF(qt_qnormalizedSignature, 1),
    CALLDEF(qt_qmethods, 1),
    CALLDEF(qt_qenums, 1),
    CALLDEF(qt_qproperties, 1),
    CALLDEF(qt_qclasses, 1),
    CALLDEF(qt_qsmokes, 0),
    
    CALLDEF(qt_qinvoke, 4),
    CALLDEF(qt_qinvokeStatic, 3),

    CALLDEF(qt_qsetProperty, 3),
    CALLDEF(qt_qproperty, 2),

    CALLDEF(qt_qcast, 2),
    CALLDEF(qt_qenclose, 2),
 
    {NULL, NULL, 0}
};


#define REG_CALLABLE(fun) R_RegisterCCallable("qtbase", #fun, (DL_FUNC) fun)

void R_init_qtbase(DllInfo *dll)
{
    init_utils(); // initializes some utilities
    init_smoke();
    init_type_handlers();
    
    // Register C routines
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);

    // Register for calling by other packages.
    REG_CALLABLE(wrapQWidget);
    REG_CALLABLE(wrapQObject);
    REG_CALLABLE(wrapPointer);
    REG_CALLABLE(unwrapQObjectReferee);
    REG_CALLABLE(unwrapQGraphicsItemReferee);
    REG_CALLABLE(unwrapQGraphicsLayoutItemReferee);
    REG_CALLABLE(wrapQGraphicsWidget);
    REG_CALLABLE(wrapQGraphicsItem);
    REG_CALLABLE(wrapQGraphicsLayoutItem);
    
    REG_CALLABLE(asStringArray);
    REG_CALLABLE(sexp2qstring);
    REG_CALLABLE(asRStringArray);
    REG_CALLABLE(qstring2sexp);

    REG_CALLABLE(asRRectF);
    REG_CALLABLE(asRMatrix);
    REG_CALLABLE(asRPointF);
    REG_CALLABLE(asRSizeF);
    REG_CALLABLE(asRColor);
    REG_CALLABLE(asRFont);

    REG_CALLABLE(asQRectF);
    REG_CALLABLE(asQMatrix);
    REG_CALLABLE(asQPointF);
    REG_CALLABLE(asQSizeF);
    REG_CALLABLE(asQColor);
    REG_CALLABLE(asQColors);
    REG_CALLABLE(asQFont);
    
    REG_CALLABLE(addQObjectReference);
    REG_CALLABLE(addQWidgetReference);
    REG_CALLABLE(addQGraphicsItemReference);
    REG_CALLABLE(addQGraphicsLayoutItemReference);
}
