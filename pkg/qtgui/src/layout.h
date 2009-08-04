#ifndef QTGUI_LAYOUT_H
#define QTGUI_LAYOUT_H

SEXP qt_qlayout(SEXP);
SEXP qt_qsetLayout_QWidget(SEXP,SEXP);
SEXP qt_qsetContentsMargins_QLayout(SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qsetSpacing_QGridLayout(SEXP,SEXP);
SEXP qt_qsetVerticalSpacing_QGridLayout(SEXP,SEXP);
SEXP qt_qsetHorizontalSpacing_QGridLayout(SEXP,SEXP);
SEXP qt_qaddWidget_QGridLayout(SEXP,SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qremoveWidget_QLayout(SEXP,SEXP);
SEXP qt_qaddLayout_QGridLayout(SEXP,SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qcolumnCount_QGridLayout(SEXP);
SEXP qt_qrowCount_QGridLayout(SEXP);

#endif

