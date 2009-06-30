

qgraphicsScene <- function()
{
    .Call(qt_qgraphicsScene)
}

qaddItem <- function(x, item, ...) UseMethod("qaddItem")

qaddItem.QGraphicsScene <- function(x, item) {
  invisible(.Call(qt_qaddItem_QGraphicsScene, x, item))
}

## SEXP qt_qaddItem_QGraphicsScene(s, item)

qsceneRect <- function(x, ...)
{
    UseMethod("qsceneRect")
}

qsceneRect.QGraphicsScene <- function(x)
{
    .Call(qt_qsceneRect_QGraphicsScene, x)
}

qsceneRect.QGraphicsView <- function(x)
{
    .Call(qt_qsceneRect_QGraphicsView, x)
}

dim.QRectF <- function(x) x[2,] - x[1,]

dim.QGraphicsScene <- function(x) dim(qsceneRect(x))

dim.QGraphicsItem <- function(x) dim(qboundingRect(x))

qboundingRect <- function(x, ...) UseMethod("qboundingRect")

qboundingRect.QGraphicsItem <- function(x) {
  .Call(qt_qboundingRect_QGraphicsItem, x)
}

`qsceneRect<-` <- function(x, ..., value)
{
    UseMethod("qsceneRect<-")
}

`qsceneRect<-.QGraphicsScene` <- function(x, value)
{
    stopifnot(is(value, "QRectF"))
    .Call(qt_qsetSceneRect_QGraphicsScene, x, value)
}

`qsceneRect<-.QGraphicsView` <- function(s, value)
{
    stopifnot(is(value, "QRectF"))
    .Call(qt_qsetSceneRect_QGraphicsView, x, value)
}

qclear <- function(x, ...)
{
    UseMethod("qclear")
}

qclear.QGraphicsScene <- function(x, ...)
{
    .Call(qt_qclear_QGraphicsScene, x)
}

qclearSelection <- function(x, ...)
{
    UseMethod("qclearSelection")
}

qclearSelection.QGraphicsScene <- function(x)
{
    .Call(qt_qclearSelection_QGraphicsScene, x)
}

qitems <- function(x, ...) UseMethod("qitems")

qitems.QGraphicsScene <- function(x, r) {
  if (is(r, "QRectF"))
    .Call(qt_qitemsInRect_QGraphicsScene, x, r)
  else if (is(r, "QPointF"))
    .Call(qt_qitemsAtPoint_QGraphicsScene, x, r)
  ## else if (is(r, "QPolygonF"))
  ##   .Call(qt_qitemsInPolygon_QGraphicsScene, x, r)
  ## else if (is(r, "QPainterPath"))
  ##   .Call(qt_qitemsInPath_QGraphicsScene, p, x)
  else stop("invalid arguments")
}

`qgeometry<-` <- function(x, value) {
### FIXME: technically QGraphicsLayoutItem, but that's not supported yet
  stopifnot(inherits(p, "QGraphicsWidget"))
  stopifnot(is(value, "QRectF"))
  invisible(.Call(qt_qsetGeometry_QGraphicsWidget, x, value))
}

qgeometry <- function(p) {
  stopifnot(inherits(p, "QGraphicsWidget"))
  .Call(qt_qgeometry_QGraphicsWidget, p)
}

## SEXP qt_qitemsBoundingRect(s)


## SEXP qt_qsetFont_QGraphicsScene(s, font)


qsetItemIndexMethod <- function(x, method = c("none", "bsptree"))
{
    method <- match.arg(method)
    .Call(qt_qsetItemIndexMethod, x, method)
}

## SEXP qt_qsetBackgroundBrush(s, brush)


## SEXP qt_setParentItem(item, parent)


## SEXP qt_setPos_QGraphicsItem(item, x, y)


## SEXP qt_setZValue(item, z)


## SEXP qt_setToolTip_QGraphicsItem(item, s)


## SEXP qt_qgraphicsProxyWidget(w)

qscene.points <- function(s, x, y, radius = 1)
{
    xy <- xy.coords(x, y, recycle = TRUE)
    .Call(scene_addPoints, s, as.double(xy$x), as.double(xy$y), as.double(radius))
}

qscene.lines <- function(s, x, y, lwd = 0)
{
    xy <- xy.coords(x, y, recycle = TRUE)
    .Call(scene_addLines, s, as.double(xy$x), as.double(xy$y), as.double(lwd))
}

qscene.segments <- function(s, x1, y1, x2, y2, lwd = 0)
{
    n <- max(length(x1), length(x2), length(y1), length(y2))
    .Call(scene_addSegments,
          s,
          rep(as.double(x1), length.out = n),
          rep(as.double(y1), length.out = n),
          rep(as.double(x2), length.out = n),
          rep(as.double(y2), length.out = n),
          as.double(lwd))
}

qscene.rect <- function(s, x, y, w = 1, h = 1)
{
    xy <- xy.coords(x, y, recycle = TRUE)
    .Call(scene_addRect, s, as.double(xy$x), as.double(xy$y), as.double(w), as.double(h))
}


qscene.text <- function(s, x, y, labels, html = FALSE)
{
    xy <- xy.coords(x, y, recycle = TRUE)
    .Call(scene_addText, s, as.double(xy$x), as.double(xy$y), as.character(labels), as.logical(html))
}


qgraphicsView <- function(rscene)
{
    .Call(qt_qgraphicsView, rscene)
}

qfitScene <- function(v)
{
    .Call(qt_qfitScene_QGraphicsView, v)
}

qsetTransform <- function(x, ...)
{
    UseMethod("qsetTransform")
}

qsetTransform.QGraphicsView <-
    function(x,
             scale = 1, xscale = scale, yscale = scale,
             rotate = 0, shear = NULL,
             translate = c(0, 0))
{
    .Call(qt_qsetTransform_QGraphicsView,
          x,
          as.double(xscale),
          as.double(yscale),
          as.double(rotate),
          as.double(rep(translate, length.out = 2)))
}

qmatrix <- function(x = matrix(c(1, 0, 0, 0, 1, 0), ncol=2), inverted = FALSE)
  UseMethod("qmatrix")

qmatrix.QGraphicsItem <- function(x, inverted = FALSE) {
  .Call(qt_qmatrix_QGraphicsItem, x, as.logical(inverted))
}

qmatrix.QGraphicsView <- function(x, inverted = FALSE) {
  .Call(qt_qmatrix_QGraphicsView, x, as.logical(inverted))
}

`qmatrix<-` <- function(x, ...) UseMethod("qmatrix<-")

`qmatrix<-.QGraphicsView` <- function(x, value) {
  .Call(qt_qsetMatrix_QGraphicsView, x, value)
}

qupdate <- function(x) UseMethod("qupdate")

qupdate.QGraphicsView <- function(x)
  invisible(.Call(qt_qupdate_QGraphicsView, x))

qupdate.QGraphicsScene <- function(x)
  invisible(.Call(qt_qupdate_QGraphicsScene, x))

qupdate.QGraphicsItem <- function(x) 
  invisible(.Call(qt_qupdate_QGraphicsItem, x))

qsetDragMode <- 
    function(v, mode = c("none", "scroll", "select"))
{
    mode <- match.arg(mode)
    imode <- switch(mode, scroll = 1L, select = 2L, 0L)
    .Call(qt_qsetDragMode, v, imode)
}

setAntialias <- function(v, mode = TRUE)
{
    .Call(view_setAntialias, v, as.logical(mode))
}

qsetItemFlags <- function(x, flag = c("movable", "selectable"), status = FALSE)
{
    flag <- match.arg(flag)
    .Call(qt_qsetItemFlags, x, flag, status)
}

`qcacheMode<-` <- function(x, value) {
  stopifnot(inherits(x, "QGraphicsItem"))
  modes <- c(none = 0L, item = 1L, device = 2L)
  mode <- modes[value]
  if (is.na(mode))
    stop("'value' must be one of ", paste(names(modes), collapse = ", "))
  invisible(.Call(qt_qsetCacheMode_QGraphicsItem, x, mode))
}

qcacheMode <- function(x) {
  stopifnot(inherits(x, "QGraphicsItem"))
  modes <- c("none", "item", "device")
  modes[.Call(qt_qcacheMode_QGraphicsItem, x) + 1]
}

qfocus <- function(x) {
  stopifnot(inherits(x, "QGraphicsItem"))
  .Call(qt_qsetFocus_QGraphicsItem, x)
}

qsetTextItemInteraction <- function(x, mode = c("none", "editor", "browser"))
{
    mode <- match.arg(mode)
    .Call(qt_qsetTextItemInteraction, x, mode)
}

## QGraphicsLayout stuff

### FIXME: no support for QGraphicsGridLayout yet
if (FALSE) {
qaddItem.QGraphicsGridLayout <-
  function(x, item, row = 0, col = 0, nrow = 1, ncol = 1)
{
  invisible(.Call(qt_qaddItem_QGraphicsGridLayout, x, item, row, col, nrow,
                  ncol))
}

"[<-.QGraphicsGridLayout" <-
  function (x, i, j, ..., value)
{
  qaddItem(x, value, row = i, col = j, ...)
}

qrowStretch <- function(p) {
  stopifnot(inherits(p, "QGraphicsGridLayout"))
  .Call(qt_qrowStretch_QGraphicsGridLayout, p)
}

qcolStretch <- function(p) {
  stopifnot(inherits(p, "QGraphicsGridLayout"))
  .Call(qt_qcolStretch_QGraphicsGridLayout, p)
}

`qrowStretch<-` <- function(p, value) {
  stopifnot(inherits(p, "QGraphicsGridLayout"))
  invisible(.Call(qt_qsetRowStretch_QGraphicsGridLayout, p,
                  as.integer(value)))
}

`qcolStretch<-` <- function(p, value) {
  stopifnot(inherits(p, "QGraphicsGridLayout"))
  invisible(.Call(qt_qsetColStretch_QGraphicsGridLayout, p,
                  as.integer(value)))
}

`qhSpacing<-` <- function(p, value) {
  stopifnot(inherits(p, "QGraphicsGridLayout"))
  invisible(.Call(qt_qsetHorizontalSpacing_QGraphicsGridLayout, p,
                  as.numeric(value)))
}

`qvSpacing<-` <- function(p, value) {
  stopifnot(inherits(p, "QGraphicsGridLayout"))
  invisible(.Call(qt_qsetVerticalSpacing_QGraphicsGridLayout, p,
                  as.numeric(value)))
}
}
