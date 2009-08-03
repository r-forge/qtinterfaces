#ifndef QTGUI_LAYOUT_H
#define QTGUI_LAYOUT_H

SEXP qt_qlayout(SEXP);
SEXP qt_qsetLayout(SEXP,SEXP);
SEXP qt_qsetContentsMarginsLayout(SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qsetSpacing(SEXP,SEXP);
SEXP qt_qsetVerticalSpacing(SEXP,SEXP);
SEXP qt_qsetHorizontalSpacing(SEXP,SEXP);
SEXP qt_qaddWidgetToLayout(SEXP,SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qremoveWidgetFromLayout(SEXP,SEXP);
SEXP qt_qaddLayoutToLayout(SEXP,SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qcolumnCount(SEXP);
SEXP qt_qrowCount(SEXP);

#endif

