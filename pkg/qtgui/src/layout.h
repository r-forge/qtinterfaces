
#ifndef QTGUI_LAYOUT_H
#define QTGUI_LAYOUT_H

SEXP qt_qlayout(SEXP x);
SEXP qt_qsetLayout(SEXP x, SEXP layout);
SEXP qt_qsetContentsMarginsLayout(SEXP x, SEXP left, SEXP top, SEXP right, SEXP bottom);
SEXP qt_qsetSpacing(SEXP x, SEXP spacing);
SEXP qt_qsetVerticalSpacing(SEXP x, SEXP spacing);
SEXP qt_qsetHorizontalSpacing(SEXP x, SEXP spacing);
SEXP qt_qaddWidgetToLayout(SEXP x, SEXP widget, SEXP row, SEXP column, SEXP nrow, SEXP ncolumn);
SEXP qt_qremoveWidgetFromLayout(SEXP x, SEXP widget);
SEXP qt_qaddLayoutToLayout(SEXP x, SEXP layout, SEXP row, SEXP column, SEXP nrow, SEXP ncolumn);
SEXP qt_qcolumnCount(SEXP x);
SEXP qt_qrowCount(SEXP x);

#endif
