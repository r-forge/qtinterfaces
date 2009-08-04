
#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QGridLayout>

#include <qtbase.h>

extern "C" {
#include "layout.h"
}

SEXP
qt_qlayout(SEXP x)
{
    if (x == R_NilValue)
	return wrapQObject(new QGridLayout());
    else {
	QLayout *l = unwrapQObject(x, QWidget)->layout();
	if (l) return wrapQObject(l);
	else return R_NilValue;
    }
}

SEXP
qt_qsetLayout_QWidget(SEXP x, SEXP layout)
{
    unwrapQObject(x, QWidget)->setLayout(unwrapQObject(layout, QLayout));
    return R_NilValue;
}


SEXP
qt_qsetContentsMargins_QLayout(SEXP x, SEXP left, SEXP top, SEXP right, SEXP bottom)
{
    unwrapQObject(x, QLayout)->
	setContentsMargins(asInteger(left),
			   asInteger(top),
			   asInteger(right),
			   asInteger(bottom));
    return R_NilValue;
}


SEXP
qt_qsetSpacing_QGridLayout(SEXP x, SEXP spacing)
{
    unwrapQObject(x, QGridLayout)->setSpacing(asInteger(spacing));
    return R_NilValue;
}

SEXP
qt_qsetVerticalSpacing_QGridLayout(SEXP x, SEXP spacing)
{
    unwrapQObject(x, QGridLayout)->setVerticalSpacing(asInteger(spacing));
    return R_NilValue;
}

SEXP
qt_qsetHorizontalSpacing_QGridLayout(SEXP x, SEXP spacing)
{
    unwrapQObject(x, QGridLayout)->setHorizontalSpacing(asInteger(spacing));
    return R_NilValue;
}

SEXP
qt_qaddWidget_QGridLayout(SEXP x, SEXP widget, 
			  SEXP row, SEXP column, 
			  SEXP nrow, SEXP ncolumn)
{
    unwrapQObject(x, QGridLayout)->
	addWidget(unwrapQObject(widget, QWidget),
		  asInteger(row)-1, asInteger(column)-1,
		  asInteger(nrow), asInteger(ncolumn));
    return R_NilValue;
}

SEXP
qt_qremoveWidget_QLayout(SEXP x, SEXP widget)
{
    unwrapQObject(x, QLayout)->removeWidget(unwrapQObject(widget, QWidget));
    return R_NilValue;
}

SEXP
qt_qaddLayout_QGridLayout(SEXP x, SEXP layout, 
			  SEXP row, SEXP column, 
			  SEXP nrow, SEXP ncolumn)
{
    unwrapQObject(x, QGridLayout)->
	addLayout(unwrapQObject(layout, QGridLayout),
		  asInteger(row)-1, asInteger(column)-1,
		  asInteger(nrow), asInteger(ncolumn));
    return R_NilValue;
}

SEXP
qt_qcolumnCount_QGridLayout(SEXP x)
{
    return ScalarInteger(unwrapQObject(x, QGridLayout)->columnCount());
}

SEXP
qt_qrowCount_QGridLayout(SEXP x)
{
    return ScalarInteger(unwrapQObject(x, QGridLayout)->rowCount());
}

// void QGridLayout::setOriginCorner ( Qt::Corner corner )

