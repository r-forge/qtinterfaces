
qaction <- function(desc = "Action", shortcut = NULL, parent = NULL,
                    tooltip = NULL, checkable = FALSE)
{
    .Call(qt_qaction, desc, shortcut, parent, tooltip, checkable)
}

qaddAction <- function(x, action)
{
    if (is.list(action))
    {
        lapply(action, function(a) qaddAction(x, a))
        return()
    }
    UseMethod("qaddAction")
}

qaddAction.QWidget <- function(x, action) .Call(qt_qaddAction_QWidget, x, action)
qaddAction.QMenu <- function(x, action) .Call(qt_qaddAction_QMenu, x, action)
qaddAction.QGraphicsWidget <- function(x, action) .Call(qt_qaddAction_QGraphicsWidget, x, action)


qsetContextMenuPolicy <-
    function(x, policy = c("none", "prevent", "default",
                           "actions", "custom"))
{
    policy <- match.arg(policy)
    .Call(qt_qsetContextMenuPolicy, x, policy)
}

## Qt::NoContextMenu
## Qt::PreventContextMenu
## Qt::DefaultContextMenu
## Qt::ActionsContextMenu
## Qt::CustomContextMenu

