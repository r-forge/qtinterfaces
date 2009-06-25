

qgraphicsScene <- function()
{
    .Call(qt_qgraphicsScene)
}



## SEXP qt_qaddItem_QGraphicsScene(s, item)

qsceneRect <- function(x)
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

qsetSceneRect <- function(x, ...)
{
    UseMethod("qsetSceneRect")
}

qsetSceneRect.QGraphicsScene <- function(x, xlim, ylim, ...)
{
    .Call(qt_qsetSceneRect_QGraphicsScene, x, xlim, ylim, ...)
}

qsetSceneRect.QGraphicsView <- function(s, xlim, ylim)
{
    .Call(qt_qsetSceneRect_QGraphicsView, x, xlim, ylim, ...)
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


qgraphicsView <- function(rscene, store = TRUE)
{
    ans <- .Call(qt_qgraphicsView, rscene)
    if (store) attr(ans, "scene") <- rscene
    ans
}

qfitScene <- function(v)
{
    .Call(qt_fitScene_QGraphicsView, v)
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

