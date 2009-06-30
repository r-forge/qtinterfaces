
qaction <- function(desc = "Action", shortcut = NULL, parent = NULL,
                    tooltip = NULL, checkable = FALSE)
{
    .Call(qt_qaction, desc, shortcut, parent, tooltip, checkable)
}

qaddAction <- function(x, action)
{
  if (is.list(action)) {
    lapply(action, function(a) qaddAction(x, a))
    return()
  }
  if (is(x, "QMenu"))
    .Call(qt_qaddActionToQMenu, x, action)
  else if (is(x, "QWidget"))
    .Call(qt_qaddActionToQWidget, x, action)
  else if (is(x, "QGraphicsWidget"))
    .Call(qt_qaddAction_QGraphicsWidget, x, action)
  invisible()
}

qsetContextMenuPolicy <-
    function(x, policy = c("none", "prevent", "default",
                           "actions", "custom"))
{
    policy <- match.arg(policy)
    .Call(qtbase:::qt_qsetContextMenuPolicy, x, policy)
}

## Qt::NoContextMenu
## Qt::PreventContextMenu
## Qt::DefaultContextMenu
## Qt::ActionsContextMenu
## Qt::CustomContextMenu
