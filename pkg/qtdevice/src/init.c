
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

SEXP do_newQtDevice(SEXP width, SEXP height, SEXP pointsize, SEXP family,
		    SEXP aaEdges, SEXP aaText, SEXP bgimage);

SEXP qt_qsceneDevice(SEXP width, SEXP height,
		     SEXP pointsize, SEXP family);

SEXP qt_qsceneView(SEXP x); 


void R_init_qtdevice(DllInfo *dll);

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static R_CallMethodDef CallEntries[] = {

    CALLDEF(do_newQtDevice, 7),
    CALLDEF(qt_qsceneDevice, 4),
    CALLDEF(qt_qsceneView, 1),

    {NULL, NULL, 0}
};


void R_init_qtdevice(DllInfo *dll)
{
    // Register C routines
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

