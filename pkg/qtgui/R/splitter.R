
qsplitter <- function(horizontal = TRUE)
{
    .Call(qt_qsplitter, horizontal)
}

qaddWidget.QSplitter <- function(x, widget, ...)
{
    .Call(qt_qaddWidget_QSplitter, x, widget)
}

qsetStretchFactor <- function(x, ...) UseMethod("qsetStretchFactor")

qsetStretchFactor.QSplitter <- function(x, index, stretch)
{
    .Call(qt_qsetStretchFactor_QSplitter, x, index, stretch)
}

