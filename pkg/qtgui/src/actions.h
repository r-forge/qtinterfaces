#ifndef QTGUI_ACTIONS_H
#define QTGUI_ACTIONS_H

SEXP qt_qaction(SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qaddActionToQWidget(SEXP,SEXP);
SEXP qt_qaddActionToQMenu(SEXP,SEXP);
SEXP qt_qsetContextMenuPolicy(SEXP,SEXP);
SEXP qt_qsetEnabled_QAction(SEXP,SEXP);
SEXP qt_qchecked_QAction(SEXP);
SEXP qt_qaddAction_QGraphicsWidget(SEXP,SEXP);

#endif

