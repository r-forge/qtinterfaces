
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QKeySequence>
#include <QGraphicsWidget>

#include <qtbase.h>

extern "C" {
#include "actions.h"
}

SEXP
qt_qaction(SEXP desc, SEXP shortcut, SEXP parent, SEXP tooltip,
           SEXP checkable)
{
    QAction *a;
    if (parent == R_NilValue)
	a = new QAction(sexp2qstring(desc), 0);
    else 
	a = new QAction(sexp2qstring(desc), unwrapQObject(parent, QWidget));
    if (shortcut != R_NilValue) {
	// a->setShortcut(sexp2qstring(shortcut));
	a->setShortcut(QKeySequence::fromString(sexp2qstring(shortcut)));
    }
    a->setCheckable(asLogical(checkable));
    a->setToolTip(sexp2qstring(tooltip));
    return wrapQObject(a);
}

SEXP
qt_qaddAction_QWidget(SEXP x, SEXP a)
{
    unwrapQObject(x, QWidget)->addAction(unwrapQObject(a, QAction));
    return R_NilValue;
}

SEXP
qt_qaddAction_QMenu(SEXP x, SEXP a)
{
    unwrapQObject(x, QMenu)->addAction(unwrapQObject(a, QAction));
    return R_NilValue;
}

SEXP 
qt_qaddAction_QGraphicsWidget(SEXP x, SEXP a) 
{
    QGraphicsWidget *item = unwrapQObject(x, QGraphicsWidget);
    item->addAction(unwrapQObject(a, QAction));
    return R_NilValue;
}


SEXP
qt_qsetContextMenuPolicy(SEXP x, SEXP policy)
{
    QString p = sexp2qstring(policy);
    QWidget *w = unwrapQObject(x, QWidget);
    if (p == "none") w->setContextMenuPolicy(Qt::NoContextMenu);
    else if (p == "prevent") w->setContextMenuPolicy(Qt::PreventContextMenu);
    else if (p ==  "default") w->setContextMenuPolicy(Qt::DefaultContextMenu);
    else if (p == "actions") w->setContextMenuPolicy(Qt::ActionsContextMenu);
    else if (p ==  "custom") w->setContextMenuPolicy(Qt::CustomContextMenu);
    return R_NilValue;
}

