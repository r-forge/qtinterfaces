
// Wrappers for the Qt Graphics View framework

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsGridLayout>

#include <qtbase.h>

extern "C" {
#include "graphicsView.h"
}


SEXP qt_qgraphicsScene()
{
    return wrapQObject(new QGraphicsScene(0));
}

SEXP qt_qaddItem_QGraphicsScene(SEXP s, SEXP ritem)
{
// #define unwrapQGraphicsItem(x, type) ({                                      \
//       type *ans = qgraphicsitem_cast<type *>(unwrapQGraphicsItemReferee(x)); \
//       if (!ans) error("unwrapQGraphicsItem: Coercion to " #type " failed"); \
//       ans;                                                              \
//     })
  QGraphicsScene *scene = unwrapQObject(s, QGraphicsScene);
  // unwrapQGraphicsItemReferee(ritem);
  QGraphicsItem *item = unwrapQGraphicsItem(ritem, QGraphicsItem); // failing
  scene->addItem(item);
  // Make sure that item is not deleted before its scene
  // FIXME: leaky -- needs to happen in itemChange() upon ItemSceneChange,
  // but that requires always using a QGraphicsWidget subclass
  addQGraphicsItemReference(item, scene);
  return s;
}

// NOTE: The QGraphicsLayout stuff is not based on QGraphicsItem nor
// QObject. This makes it difficult to check types safely and, much
// worse, there is no easy way to manage their memory. See notes in
// Reference.cpp.
SEXP qt_qaddItem_QGraphicsGridLayout(SEXP rself, SEXP ritem, SEXP rrow,
                                     SEXP rcol, SEXP rrowSpan, SEXP rcolSpan)
{
  QGraphicsGridLayout *layout = unwrapPointer(rself, QGraphicsGridLayout);
  QGraphicsLayoutItem *item = unwrapPointer(ritem, QGraphicsLayoutItem);
  layout->addItem(item, asInteger(rrow), asInteger(rcol), asInteger(rrowSpan),
                  asInteger(rcolSpan));
  return rself;
}

SEXP qt_qcolStretch_QGraphicsGridLayout(SEXP rself) {
  QGraphicsGridLayout *layout = unwrapPointer(rself, QGraphicsGridLayout);
  SEXP ans = allocVector(INTSXP, layout->columnCount());
  for (int i = 0; i < length(ans); i++) {
    INTEGER(ans)[i] = layout->columnStretchFactor(i);
  }
  return ans;
}
SEXP qt_qrowStretch_QGraphicsGridLayout(SEXP rself) {
  QGraphicsGridLayout *layout = unwrapPointer(rself, QGraphicsGridLayout);
  SEXP ans = allocVector(INTSXP, layout->rowCount());
  for (int i = 0; i < length(ans); i++) {
    INTEGER(ans)[i] = layout->rowStretchFactor(i);
  }
  return ans;
}
  
SEXP qt_qsetColStretch_QGraphicsGridLayout(SEXP rself, SEXP rstretch) {
  QGraphicsGridLayout *layout = unwrapPointer(rself, QGraphicsGridLayout);
  for (int i = 0; i < length(rstretch); i++) {
    layout->setColumnStretchFactor(i, INTEGER(rstretch)[i]);
  }
  return rself;
}
SEXP qt_qsetRowStretch_QGraphicsGridLayout(SEXP rself, SEXP rstretch) {
  QGraphicsGridLayout *layout = unwrapPointer(rself, QGraphicsGridLayout);
  for (int i = 0; i < length(rstretch); i++) {
    layout->setRowStretchFactor(i, INTEGER(rstretch)[i]);
  }
  return rself;
}

SEXP qt_qsetHorizontalSpacing_QGraphicsGridLayout(SEXP rself, SEXP rspacing) {
  QGraphicsGridLayout *layout = unwrapPointer(rself, QGraphicsGridLayout);
  layout->setHorizontalSpacing(asReal(rspacing));
  return rself;
}
SEXP qt_qsetVerticalSpacing_QGraphicsGridLayout(SEXP rself, SEXP rspacing) {
  QGraphicsGridLayout *layout = unwrapPointer(rself, QGraphicsGridLayout);
  layout->setVerticalSpacing(asReal(rspacing));
  return rself;
}

SEXP qt_qsceneRect_QGraphicsScene(SEXP x)
{
    SEXP ans = PROTECT(allocVector(REALSXP, 4));
    double *ansp = REAL(ans);
    QRectF rect = unwrapQObject(x, QGraphicsScene)->sceneRect();
    ansp[0] = rect.x();
    ansp[1] = rect.y();
    ansp[2] = rect.width();
    ansp[3] = rect.height();
    UNPROTECT(1);
    return ans;
}

SEXP qt_qsceneRect_QGraphicsView(SEXP x)
{
    SEXP ans = PROTECT(allocVector(REALSXP, 4));
    double *ansp = REAL(ans);
    QRectF rect = unwrapQObject(x, QGraphicsView)->sceneRect();
    ansp[0] = rect.x();
    ansp[1] = rect.y();
    ansp[2] = rect.width();
    ansp[3] = rect.height();
    UNPROTECT(1);
    return ans;
}

SEXP qt_qsetSceneRect_QGraphicsScene(SEXP s, SEXP rrect)
{
    unwrapQObject(s, QGraphicsScene)->setSceneRect(asQRectF(rrect));
    return s;
}

SEXP qt_qsetSceneRect_QGraphicsView(SEXP s, SEXP rrect)
{
    unwrapQObject(s, QGraphicsView)->setSceneRect(asQRectF(rrect));
    return s;
}

SEXP qt_qclear_QGraphicsScene(SEXP s)
{
    unwrapQObject(s, QGraphicsScene)->clear();
    return R_NilValue;
}

SEXP qt_qclearSelection_QGraphicsScene(SEXP s)
{
    unwrapQObject(s, QGraphicsScene)->clearSelection();
    return R_NilValue;
}

SEXP qt_qitemsBoundingRect(SEXP s)
{
    return R_NilValue;
}

SEXP qt_qsetFont_QGraphicsScene(SEXP s, SEXP font)
{
    return R_NilValue;
}

SEXP qt_qsetItemIndexMethod(SEXP s, SEXP method)
{
    if (sexp2qstring(method) == "none")
	unwrapQObject(s, QGraphicsScene)->setItemIndexMethod(QGraphicsScene::NoIndex);
    else 
	unwrapQObject(s, QGraphicsScene)->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    return R_NilValue;
}

SEXP qt_qsetBackgroundBrush(SEXP s, SEXP brush)
{
    return R_NilValue;
}


SEXP qt_qsetParentItem(SEXP item, SEXP parent)
{
    unwrapQGraphicsItem(item, QGraphicsItem)->
	setParentItem(unwrapQGraphicsItem(parent, QGraphicsItem));
    return R_NilValue;
}

SEXP qt_qsetPos_QGraphicsItem(SEXP item, SEXP rpoint)
{
  unwrapQGraphicsItem(item, QGraphicsItem)->setPos(asQPointF(rpoint));
  return item;
}

SEXP qt_qsetZValue(SEXP item, SEXP z)
{
    unwrapQGraphicsItem(item, QGraphicsItem)->setZValue(asReal(z));
    return item;
}

SEXP qt_qsetMinimumSize_QGraphicsLayoutItem(SEXP ritem, SEXP rsize)
{
  QGraphicsLayoutItem *item =
    unwrapQGraphicsLayoutItem(ritem, QGraphicsLayoutItem);
  item->setMinimumSize(asQSizeF(rsize));
  return ritem;
}

SEXP qt_qminimumSize_QGraphicsLayoutItem(SEXP ritem)
{
  QGraphicsLayoutItem *item =
    unwrapQGraphicsLayoutItem(ritem, QGraphicsLayoutItem);
  return asRSizeF(item->minimumSize());
}

SEXP qt_qsetToolTip_QGraphicsItem(SEXP item, SEXP s)
{
    // unwrapQObject(item, QGraphicsItem)->setToolTip(sexp2qstring(s));
    return R_NilValue;
}

SEXP qt_qsetCacheMode_QGraphicsItem(SEXP rself, SEXP rmode) {
  QGraphicsItem *item = unwrapQGraphicsItem(rself, QGraphicsItem);
  item->setCacheMode((QGraphicsItem::CacheMode)asInteger(rmode));
  return rself;
}
SEXP qt_qcacheMode_QGraphicsItem(SEXP rself) {
  QGraphicsItem *item = unwrapQGraphicsItem(rself, QGraphicsItem);
  return ScalarInteger(item->cacheMode());
}

SEXP qt_qsetFocus_QGraphicsItem(SEXP rself) {
  QGraphicsItem *item = unwrapQGraphicsItem(rself, QGraphicsItem);
  item->setFocus();
  return rself;
}


// built-in shape items

// helper functions that should maybe move to qtbase/src/convert.*

enum GPARS {
    COLOR, FILL, LWD, PENSTYLE,
    FILLSTYLE, PENCAP, PENJOIN
};

static QBrush asQBrush(SEXP pars)
{
    return 
	QBrush(asQColor(VECTOR_ELT(pars, FILL)), 
	       (enum Qt::BrushStyle) asInteger(VECTOR_ELT(pars, FILLSTYLE)));
}

static QPen asQPen(SEXP pars)
{
    // pencap *= 0x10
    // penjoin *= 0x40
    return 
	QPen(QBrush(asQColor(VECTOR_ELT(pars, FILL))),
	     asReal(VECTOR_ELT(pars, LWD)), 
	     (enum Qt::PenStyle) asInteger(VECTOR_ELT(pars, PENSTYLE)));
    // doesn't work: how to convert? 
	 // (enum Qt::PenCapStyle) 0x10 * asInteger(VECTOR_ELT(pars, PENCAP)),
	 // (enum Qt::PenJoinStyle) 0x40 * asInteger(VECTOR_ELT(pars, PENJOIN)));
}


SEXP qt_qgraphicsEllipseItem(SEXP x, SEXP y, 
			     SEXP width, SEXP height, 
			     SEXP pars)
{
    double 
	px = asReal(x), py = asReal(y),
	pw = asReal(width), ph = asReal(height);
    QGraphicsEllipseItem *item = 
	new QGraphicsEllipseItem(px - pw/2, py - ph/2, pw, ph, 0);
    item->setBrush(asQBrush(pars));
    item->setPen(asQPen(pars));
    return wrapQGraphicsItem(item);
}

SEXP qt_qgraphicsLineItem(SEXP x1, SEXP y1, SEXP x2, SEXP y2, 
			  SEXP pars)
{
    double 
	px1 = asReal(x1), py1 = asReal(y1),
	px2 = asReal(x2), py2 = asReal(y2);
    QGraphicsLineItem *item = 
	new QGraphicsLineItem(px1, py1, px2, py2, 0);
    item->setPen(asQPen(pars));
    return wrapQGraphicsItem(item);
}



// Can we use either wrapQObject or wrapQGraphicsItem, or is one
// preferred?

SEXP qt_qgraphicsProxyWidget(SEXP w)
{
    QGraphicsProxyWidget *pw = new QGraphicsProxyWidget(0, 0);
    pw->setWidget(unwrapQObject(w, QWidget));
    return wrapQObject(pw);
}

SEXP qt_qsetGeometry_QGraphicsWidget(SEXP rself, SEXP rx)
{
  QGraphicsWidget *widget = unwrapQObject(rself, QGraphicsWidget);
  widget->setGeometry(asQRectF(rx));
  return rself;
}

SEXP qt_qgeometry_QGraphicsWidget(SEXP rself) {
  QGraphicsWidget *widget = unwrapQObject(rself, QGraphicsWidget);
  return asRRectF(widget->geometry());
}

SEXP qt_qboundingRect_QGraphicsItem(SEXP rself) {
  QGraphicsItem *item = unwrapQGraphicsItem(rself, QGraphicsItem);
  return asRRectF(item->boundingRect());
}

SEXP qt_qitemsAtPoint_QGraphicsScene(SEXP rself, SEXP rpoint) {
  QGraphicsScene *self = unwrapQObject(rself, QGraphicsScene);
  SEXP ans;
  QList<QGraphicsItem *> items = self->items(asQPointF(rpoint));
  ans = allocVector(VECSXP, items.size());
  for (int i = 0; i < length(ans); i++)
    SET_VECTOR_ELT(ans, i, wrapQGraphicsItem(items[i]));
  return ans;
}

SEXP qt_qitemsInRect_QGraphicsScene(SEXP rself, SEXP rrect) {
  QGraphicsScene *self = unwrapQObject(rself, QGraphicsScene);
  SEXP ans;
  QList<QGraphicsItem *> items = self->items(asQRectF(rrect));
  ans = allocVector(VECSXP, items.size());
  for (int i = 0; i < length(ans); i++)
    SET_VECTOR_ELT(ans, i, wrapQGraphicsItem(items[i]));
  return ans;
}

// printing: render() has scene and view methods

// item groups: necessary? Maybe for common tooltips.  Otherwise
// parenting should be enough.

// practical stuff: ellipses, lines, etc.  Should have direct wrappers
// for built-in classes, as well as vectorized R versions.

// the cool stuff: layouts and graphicsWidgets.



SEXP
scene_addPoints(SEXP scene, SEXP x, SEXP y, SEXP radius)
{
    QGraphicsScene* s = unwrapQObject(scene, QGraphicsScene);
    int i, n = length(x);
    for (i = 0; i < n; i++) {
	// QGraphicsEllipseItem *item = s->addEllipse(REAL(x)[i], REAL(y)[i], REAL(radius)[0], REAL(radius)[0]);
	QGraphicsEllipseItem *item = s->addEllipse(0.0, 0.0, REAL(radius)[0], REAL(radius)[0]);
	item->setPos(REAL(x)[i], REAL(y)[i]);
	item->setFlags(QGraphicsItem::ItemIsSelectable | 
		       QGraphicsItem::ItemIgnoresTransformations);
    }
    return R_NilValue;
}


SEXP
scene_addLines(SEXP scene, SEXP x, SEXP y, SEXP lwd)
{
    QGraphicsScene* s = unwrapQObject(scene, QGraphicsScene);
    int nlwd = length(lwd);
    int i, n = length(x);
    for (i = 1; i < n; i++) {
	QGraphicsLineItem *item = s->addLine(REAL(x)[i-1], REAL(y)[i-1], 
					     REAL(x)[i],   REAL(y)[i], 
					     QPen(Qt::black, REAL(lwd)[(i-1) % nlwd]));
	// 	    item->setFlags(QGraphicsItem::ItemIsSelectable | 
	// 			   QGraphicsItem::ItemIgnoresTransformations);
    }
    return R_NilValue;
}


SEXP
scene_addSegments(SEXP scene, SEXP x1, SEXP y1, SEXP x2, SEXP y2, SEXP lwd)
{
    QGraphicsScene* s = unwrapQObject(scene, QGraphicsScene);
    int nlwd = length(lwd);
    int i, n = length(x1);
    for (i = 0; i < n; i++) {
	QGraphicsLineItem *item = s->addLine(REAL(x1)[i], REAL(y1)[i], REAL(x2)[i], REAL(y2)[i], 
					     QPen(Qt::black, REAL(lwd)[(i-1) % nlwd]));
	// 	    item->setFlags(QGraphicsItem::ItemIsSelectable | 
	// 			   QGraphicsItem::ItemIgnoresTransformations);
    }
    return R_NilValue;
}



// QGraphicsPolygonItem * addPolygon ( const QPolygonF & polygon, const QPen & pen = QPen(), const QBrush & brush = QBrush() )


SEXP
scene_addRect(SEXP scene, SEXP x, SEXP y, SEXP w, SEXP h)
{
    QGraphicsScene* s = unwrapQObject(scene, QGraphicsScene);
    int nw = length(w), nh = length(h);
    int i, n = length(x);
    for (i = 0; i < n; i++) {
	QGraphicsRectItem *item = s->addRect(REAL(x)[i], REAL(y)[i], 
					     REAL(w)[i % nw], REAL(h)[i % nh]);
	// 	    item->setFlags(QGraphicsItem::ItemIsSelectable | 
	// 			   QGraphicsItem::ItemIgnoresTransformations);
    }
    return R_NilValue;
}

SEXP
scene_addText(SEXP scene, SEXP x, SEXP y, SEXP labels, SEXP html)
{
    QGraphicsScene* s = unwrapQObject(scene, QGraphicsScene);
    int nlab = length(labels);
    int i, n = length(x);
    for (i = 0; i < n; i++) {
	QGraphicsTextItem *ti = s->addText(QString());
	ti->setFont(QFont("Arial"));
	if (LOGICAL(html)[0]) {
	    ti->setHtml(QString::fromLocal8Bit(CHAR(asChar(STRING_ELT(labels, i % nlab)))));
	    ti->setOpenExternalLinks(true);
	    ti->setToolTip("I am HTML!");
	}
	else {
	    ti->setPlainText(QString::fromLocal8Bit(CHAR(asChar(STRING_ELT(labels, i % nlab)))));
	}
	ti->setPos(REAL(x)[i], REAL(y)[i]);
	ti->setFlags(QGraphicsItem::ItemIsMovable | 
		     QGraphicsItem::ItemIsSelectable | 
		     QGraphicsItem::ItemIsFocusable | 
		     QGraphicsItem::ItemIgnoresTransformations);
    }
    return R_NilValue;
}




SEXP
qt_qgraphicsView(SEXP rscene)
{
    QGraphicsScene *scene = unwrapQObject(rscene, QGraphicsScene);
    QGraphicsView *view = new QGraphicsView(scene, 0);
    addQObjectReference(scene, view);
    return wrapQWidget(view);
}

SEXP
qt_qfitScene_QGraphicsView(SEXP v) {
  QGraphicsView *view = unwrapQObject(v, QGraphicsView);
  view->fitInView(view->sceneRect());
  return R_NilValue;
}

SEXP
qt_qsetTransform_QGraphicsView(SEXP v,
			       SEXP xscale,
			       SEXP yscale,
			       SEXP rotate,
			       SEXP translate)
{
    QGraphicsView *view = unwrapQObject(v, QGraphicsView);
    // shear ( qreal sh, qreal sv )
    view->scale(REAL(xscale)[0], REAL(yscale)[0]);
    view->rotate(REAL(rotate)[0]);
    view->translate(REAL(translate)[0], REAL(translate)[1]);
    return R_NilValue;
}

SEXP
qt_qsetMatrix_QGraphicsView(SEXP extp, SEXP rmatrix)
{
  QGraphicsView *view = unwrapQObject(extp, QGraphicsView);
  view->setMatrix(asQMatrix(rmatrix));
  return extp;
}
SEXP qt_qmatrix_QGraphicsView(SEXP extp, SEXP inverted)
{
  QGraphicsView *view = unwrapQObject(extp, QGraphicsView);
  return asRMatrix(view->matrix(), asLogical(inverted));
}

// just data to parent (layout/scene) coordinates, for size calculations
SEXP qt_qmatrix_QGraphicsItem(SEXP rself, SEXP rinverted) {
  QGraphicsItem *self = unwrapQGraphicsItem(rself, QGraphicsItem);
  return asRMatrix(self->transform().toAffine(), asLogical(rinverted));
}

SEXP qt_qsetMatrix_QGraphicsItem(SEXP rself, SEXP rmatrix) {
  QGraphicsItem *self = unwrapQGraphicsItem(rself, QGraphicsItem);
  self->setTransform(QTransform(asQMatrix(rmatrix)));
  return rself;
}

// FIXME: better to qupdate(scene()) and then repaint the viewport widget?
SEXP qt_qupdate_QGraphicsView(SEXP rself) {
  QGraphicsView *view = unwrapQObject(rself, QGraphicsView);
  view->scene()->update();
  view->viewport()->repaint();
  return rself;
}

SEXP qt_qupdate_QGraphicsItem(SEXP rself) {
  QGraphicsItem *item = unwrapQGraphicsItem(rself, QGraphicsItem);
  // HACK: purge the cache before updating. QGraphicsScene does not
  // seem to update the cache properly when there are multiple
  // views. This is not very efficient, but usually one is not
  // caching items that are frequently updated.
  QGraphicsItem::CacheMode mode = item->cacheMode();
  item->setCacheMode(QGraphicsItem::NoCache);
  item->setCacheMode(mode);
  item->update();
  return rself;
}

SEXP qt_qupdate_QGraphicsScene(SEXP rself) {
  QGraphicsScene *scene = unwrapQObject(rself, QGraphicsScene);
  QList<QGraphicsItem *> items = scene->items();
  for (int i = 0; i < items.size(); i++) {
    QGraphicsItem *item = items[i];
    QGraphicsItem::CacheMode mode = item->cacheMode();
    item->setCacheMode(QGraphicsItem::NoCache);
    item->setCacheMode(mode);
  }
  scene->update();
  return rself;
}

SEXP
qt_qsetDragMode(SEXP v, SEXP mode)
{
    QGraphicsView *view = unwrapQObject(v, QGraphicsView);
    if (view) {
	int m = INTEGER(mode)[0];
	if (m == 0)
	    view->setDragMode(QGraphicsView::NoDrag);
	else if (m == 1)
	    view->setDragMode(QGraphicsView::ScrollHandDrag);
	else if (m == 2)
	    view->setDragMode(QGraphicsView::RubberBandDrag);
    }
    return R_NilValue;
}

SEXP
view_setAntialias(SEXP v, SEXP mode)
{
    QGraphicsView *view = unwrapQObject(v, QGraphicsView);
    if (LOGICAL(mode)[0]) {
	view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    }
    else {
	view->setRenderHints(QPainter::TextAntialiasing);
    }
    return R_NilValue;
}



SEXP qt_qsetItemFlags(SEXP x, SEXP flag, SEXP status)
{
    QList<QGraphicsItem*> ilist = unwrapQObject(x, QGraphicsScene)->items();
    bool bstatus = (bool) asLogical(status);
    if (sexp2qstring(flag) == "movable") {
	for (int i = 0; i < ilist.size(); ++i) {
	    ilist[i]->setFlag(QGraphicsItem::ItemIsMovable, bstatus);
	}
    }
    else if (sexp2qstring(flag) == "selectable") {
	for (int i = 0; i < ilist.size(); ++i) {
	    ilist[i]->setFlag(QGraphicsItem::ItemIsSelectable, bstatus);
	}
    }
    return R_NilValue;
}


SEXP qt_qsetTextItemInteraction(SEXP x, SEXP mode)
{
    QList<QGraphicsItem*> ilist = unwrapQObject(x, QGraphicsScene)->items();
    QGraphicsTextItem *textitem;
    QString smode = sexp2qstring(mode);
    if (smode == "none") {
	for (int i = 0; i < ilist.size(); ++i) {
	    textitem = qgraphicsitem_cast<QGraphicsTextItem *>(ilist[i]);
	    if (textitem)
		textitem->setTextInteractionFlags(Qt::NoTextInteraction);
	}
    }
    else if (smode == "editor") {
	for (int i = 0; i < ilist.size(); ++i) {
	    textitem = qgraphicsitem_cast<QGraphicsTextItem *>(ilist[i]);
	    if (textitem)
		textitem->setTextInteractionFlags(Qt::TextEditorInteraction);
	}
    }
    else if (smode == "browser") {
	for (int i = 0; i < ilist.size(); ++i) {
	    textitem = qgraphicsitem_cast<QGraphicsTextItem *>(ilist[i]);
	    if (textitem)
		textitem->setTextInteractionFlags(Qt::TextBrowserInteraction);
	}
    }
    return R_NilValue;
}
