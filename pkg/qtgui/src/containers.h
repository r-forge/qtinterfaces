#ifndef QTGUI_CONTAINERS_H
#define QTGUI_CONTAINERS_H

SEXP qt_qtabWidget();
SEXP qt_qaddTab(SEXP,SEXP,SEXP,SEXP);
SEXP qt_qremoveTab(SEXP,SEXP);
SEXP qt_qstackedWidget();
SEXP qt_qaddWidgetToStack(SEXP,SEXP,SEXP);
SEXP qt_qremoveWidgetFromStack(SEXP,SEXP);
SEXP qt_qcurrentIndex(SEXP);
SEXP qt_qsetCurrentIndex(SEXP,SEXP);

#endif

