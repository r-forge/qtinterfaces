
qlayout <- function(x = NULL) .Call(qt_qlayout, x)

qsetLayout <- function(x, layout) UseMethod("qsetLayout")
qsetContentsMargins <- function(x, ...) UseMethod("qsetContentsMargins")
qsetSpacing <- function(x, spacing) UseMethod("qsetSpacing")
qsetVerticalSpacing <- function(x, spacing) UseMethod("qsetVerticalSpacing")
qsetHorizontalSpacing <- function(x, spacing) UseMethod("qsetHorizontalSpacing")

qaddWidget <- function(x, widget, ...) UseMethod("qaddWidget")
qremoveWidget <- function(x, widget) UseMethod("qremoveWidget")
qaddLayout <- function(x, widget, ...) UseMethod("qaddWidget")

qcolumnCount <- function(x) UseMethod("qcolumnCount")
qrowCount <- function(x) UseMethod("qrowCount")

qdim <- function(x) c(qrowCount(x), qcolumnCount(x))


qsetLayout.QWidget <- function(x, layout)
    .Call(qt_qsetLayout_QWidget, x, layout)

qsetContentsMargins.QLayout <-
    function(x, left, top, right, bottom, ...)
{
    .Call(qt_qsetContentsMargins_QLayout,
          x, left, top, right, bottom)
}

qsetSpacing.QGridLayout <- function(x, spacing) .Call(qt_qsetSpacing_QGridLayout, x, spacing)
qsetVerticalSpacing.QGridLayout <- function(x, spacing) .Call(qt_qsetVerticalSpacing_QGridLayout, x, spacing)
qsetHorizontalSpacing.QGridLayout <- function(x, spacing) .Call(qt_qsetHorizontalSpacing_QGridLayout, x, spacing)

qaddWidget.QGridLayout <- function(x, widget, row, column, nrow = 1L, ncolumn = 1L, ...)
{
    .Call(qt_qaddWidget_QGridLayout, x, widget, row, column, nrow, ncolumn)
}

qremoveWidget.QLayout <- function(x, widget)
{
    .Call(qt_qremoveWidget_QLayout, x, widget)
}

qaddLayout.QGridLayout <- function(x, layout, row, column, nrow = 1L, ncolumn = 1L, ...)
{
    .Call(qt_qaddLayout_QGridLayout, x, layout, row, column, nrow, ncolumn)
}

qcolumnCount.QGridLayout <- function(x) .Call(qt_qcolumnCount_QGridLayout, x)
qrowCount.QGridLayout <- function(x) .Call(qt_qrowCount_QGridLayout, x)

