
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

SEXP qt_qconnect(SEXP x, SEXP user_data, SEXP handler, SEXP which);
SEXP qt_qdisconnect(SEXP x, SEXP receiver);

void R_init_qtconnect(DllInfo *dll);

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static R_CallMethodDef CallEntries[] = {

    CALLDEF(qt_qconnect, 4),
    CALLDEF(qt_qdisconnect, 2),

    {NULL, NULL, 0}
};


void R_init_qtconnect(DllInfo *dll)
{
    // Register C routines
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

