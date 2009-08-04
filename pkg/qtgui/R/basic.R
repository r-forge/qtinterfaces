

qwidget <- function() .Call(qt_qwidget) 

qpushButton <- function(label = "Button") .Call(qt_qpushButton, label)

qlabel <- function(label = "Label") .Call(qt_qlabel, label)

qlineEdit <- function(text = "") .Call(qt_qlineEdit, text)

qcheckBox <- function(label = "Checkbox") .Call(qt_qcheckBox, as.character(label)[1])


qtext <- function(x) UseMethod("qtext")

qtext.QAbstractButton <- function(x) .Call(qt_qtext_QAbstractButton, x)

qtext.QLabel <- function(x) .Call(qt_qtext_QLabel, x)

qtext.QLineEdit <- function(x) .Call(qt_qtext_QLineEdit, x)

qtext.QGraphicsItem <- function(x) .Call(qt_qtext_QGraphicsItem, x)


qsetText <- function(x, s) UseMethod("qsetText")

qsetText.QAbstractButton <- function(x, s = "") .Call(qt_qsetText_QAbstractButton, x, s)

qsetText.QLabel <- function(x, s = "") .Call(qt_qsetText_QLabel, x, s)

qsetText.QLineEdit <- function(x, s = "") .Call(qt_qsetText_QLineEdit, x, s)

qsetText.QGraphicsItem <- function(x, s = "") .Call(qt_qsetText_QGraphicsItem, x, s)

