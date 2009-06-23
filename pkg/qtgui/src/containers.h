#ifndef QTGUI_CONTAINERS_H
#define QTGUI_CONTAINERS_H


SEXP qt_qtabWidget();
SEXP qt_qaddTab(SEXP x, SEXP tab, SEXP label, SEXP index);
SEXP qt_qremoveTab(SEXP x, SEXP index);
SEXP qt_qstackedWidget();
SEXP qt_qaddWidgetToStack(SEXP x, SEXP w, SEXP index);
SEXP qt_qremoveWidgetFromStack(SEXP x, SEXP index);
SEXP qt_qcurrentIndex(SEXP x);
SEXP qt_qsetCurrentIndex(SEXP x, SEXP index);


#endif


