
#include <QSplitter>

#include <qtbase.h>

extern "C" {
#include "splitter.h"
}

SEXP
qt_qsplitter(SEXP horizontal)
{
    if (asLogical(horizontal))
	return wrapQObject(new QSplitter(Qt::Horizontal));
    else 
	return wrapQObject(new QSplitter(Qt::Vertical));
}

SEXP
qt_qaddWidget_QSplitter(SEXP x, SEXP widget)
{
    unwrapQObject(x, QSplitter)->addWidget(unwrapQWidget(widget));
    return R_NilValue;
}

SEXP 
qt_qsetStretchFactor_QSplitter(SEXP x, SEXP index, SEXP stretch)
{
    unwrapQObject(x, QSplitter)->setStretchFactor(asInteger(index), asInteger(stretch));
    return R_NilValue;
}


