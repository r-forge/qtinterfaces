qtextExtents <- function(x, ...) UseMethod("qtextExtents")

qtextExtents.QFont <- function(x, text) {
  .Call(qt_qtextExtents_QFont, x, text)
}
