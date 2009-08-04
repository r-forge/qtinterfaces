
qshow <- function(x) UseMethod("qshow")
qclose <- function(x) UseMethod("qclose")
qraise <- function(x) UseMethod("qraise")
qlower <- function(x) UseMethod("qlower")
qparent <- function(x) UseMethod("qparent")
qsetParent <- function(x, p) UseMethod("qsetParent")
qresize <- function(x, w, h) UseMethod("qresize")
qheight <- function(x) UseMethod("qheight")
qwidth <- function(x) UseMethod("qwidth")
qsetMinimumSize <- function(x, w, h) UseMethod("qsetMinimumSize")

qsetExpanding <- function(x) UseMethod("qsetExpanding")
qisEnabled <- function(x) UseMethod("qisEnabled")
qsetEnabled <- function(x, status) UseMethod("qsetEnabled")
qwindowTitle <- function(x) UseMethod("qwindowTitle")
qsetWindowTitle <- function(x, title) UseMethod("qsetWindowTitle")
qrender <- function(x, file) UseMethod("qrender")

qrenderToPixmap <- function(x, file)
{
    .Call(qt_qrenderToPixmap, x, as.character(file)[1])
}

qrenderToSVG <- function(x, file)
{
    .Call(qt_qrenderToSVG, x, as.character(file)[1])
}


qshow.QWidget <- function(x)
{
    .Call(qt_qshow_QWidget, x)
}

qupdate.QWidget <- function(x) {
  .Call(qt_qupdate_QWidget, x)
}

print.QWidget <- function(x, ...)
{
    qshow(x)
    invisible(x)
}

qclose.QWidget <- function(x) 
{
    .Call(qt_qclose_QWidget, x)
}

qlower.QWidget <- function(x) 
{
    .Call(qt_qlower_QWidget, x)
}

qraise.QWidget <- function(x) 
{
    .Call(qt_qraise_QWidget, x)
}

qparent.QWidget <- function(x)
{
    .Call(qt_qparent_QWidget, x)
}

qsetParent.QWidget <- function(x, parent)
{
    .Call(qt_qsetParent_QWidget, x, parent)
}

qresize.QWidget <- function(x, w = NULL, h = NULL)
{
    .Call(qt_qresize_QWidget, x, w, h)
}

qheight.QWidget <- function(x)
{
    .Call(qt_qheight_QWidget, x)
}

qwidth.QWidget <- function(x)
{
    .Call(qt_qwidth_QWidget, x)
}

qsetMinimumSize.QWidget <- function(x, w, h)
{
    .Call(qt_qsetMinimumSize_QWidget, x, w, h)
}

qsetExpanding.QWidget <- function(x, vertical = NULL, horizontal = NULL)
{
    .Call(qt_qsetExpanding_QWidget, x, vertical, horizontal)
}

qsetContentsMargins.QWidget <- function(x, left, top, right, bottom, ...)
{
    .Call(qt_qsetContentsMargins_QWidget,
          x, left, top, right, bottom)
}

qisEnabled.QWidget <- function(x)
{
    .Call(qt_qisEnabled_QWidget, x)
}

qsetEnabled.QWidget <- function(x, status)
{
    .Call(qt_qsetEnabled_QWidget, x, status)
}

qwindowTitle.QWidget <- function(x)
{
    .Call(qt_qwindowTitle_QWidget, x)
}

qsetWindowTitle.QWidget <- function(x, title)
{
    .Call(qt_qsetWindowTitle_QWidget, x, title)
}

qrender.QWidget <- function(x, file = NULL)
{
    .Call(qt_qrender_QWidget, x, file)
}

qrender.QGraphicsView <- function(x, file = NULL)
{
    if (!missing(file)) warning("file specification ignored")
    .Call(qt_qrender_QGraphicsView, x)
}

