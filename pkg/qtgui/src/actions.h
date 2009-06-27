
#ifndef QTGUI_ACTIONS_H
#define QTGUI_ACTIONS_H

SEXP qt_qaction(SEXP desc, SEXP shortcut, SEXP parent, SEXP tooltip,
                SEXP checkable);
SEXP qt_qaddActionToQWidget(SEXP w, SEXP a);
SEXP qt_qaddAction_QGraphicsWidget(SEXP w, SEXP a);
SEXP qt_qaddActionToQMenu(SEXP w, SEXP a);
SEXP qt_qsetContextMenuPolicy(SEXP x, SEXP policy);

#endif
