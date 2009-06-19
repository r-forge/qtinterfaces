
#include <R.h>
#include <Rinternals.h>
#include <qtbase.h>

#include <QObject>
#include <QAbstractButton>
#include <QAction>

#include "handler.hpp"

extern "C" {
    SEXP qt_qconnect(SEXP x, SEXP user_data, SEXP handler, SEXP which);
    SEXP qt_qdisconnect(SEXP x, SEXP receiver);
}

SEXP qt_qconnect(SEXP x, SEXP user_data, SEXP handler, SEXP which)
{
    RSignalHandler *h;
    h = new RSignalHandler(unwrapQObject(x, QObject),
			   user_data, handler, which);
    return wrapQObject(h);
}

SEXP qt_qdisconnect(SEXP x, SEXP receiver)
{
    if (receiver == R_NilValue) {
	bool status = unwrapQObject(x, QObject)->disconnect();
	if (!status) warning("Disconnect unsuccessful");
    }
    else {
	bool status = unwrapQObject(x, QObject)->disconnect(unwrapQObject(receiver, RSignalHandler));
	if (!status) warning("Disconnect unsuccessful");
    }
    return R_NilValue;
}


