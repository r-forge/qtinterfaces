
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>


SEXP qt_qwebView(SEXP url);
SEXP qt_qsetUrl(SEXP x, SEXP url);
SEXP qt_qurl(SEXP x);

void R_init_qtwebview(DllInfo *dll);

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static R_CallMethodDef CallEntries[] = {

    CALLDEF(qt_qwebView, 1),
    CALLDEF(qt_qsetUrl, 2),
    CALLDEF(qt_qurl, 1),

    {NULL, NULL, 0}
};


void R_init_qtwebview(DllInfo *dll)
{
    // Register C routines
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

