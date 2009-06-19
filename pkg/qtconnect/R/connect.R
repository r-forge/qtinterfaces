
qconnect <-
    function(x, user.data = NULL, handler = NULL,
             which = c("clicked_bool", "triggered_bool",
                       "destroyed_qobject",
                       "cellActivated_int_int",
                       "cellClicked_int_int",
                       "cellDoubleClicked_int_int",
                       "itemClicked_qlistwidgetitem",
                       "itemActivated_qlistwidgetitem"))
{
    which <- match.arg(which)
    .Call(qt_qconnect, x, user.data, handler, which)
}

qdisconnect <- function(x, receiver = NULL)
{
    .Call(qt_qdisconnect, x, receiver)
}

