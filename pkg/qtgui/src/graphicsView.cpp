
// Wrappers for the Qt Graphics View framework

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>

#include <qtbase.h>

extern "C" {
#include "graphicsView.h"
}


SEXP qt_qgraphicsScene()
{
    return wrapQObject(new QGraphicsScene(0));
}

SEXP qt_qaddItem_QGraphicsScene(SEXP s, SEXP item)
{
    return R_NilValue;
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

SEXP qt_qsetSceneRect_QGraphicsScene(SEXP s, SEXP xlim, SEXP ylim)
{
    double *xl = REAL(xlim), *yl = REAL(ylim);
    unwrapQObject(s, QGraphicsScene)->setSceneRect(xl[0], yl[0], xl[1] - xl[0], yl[1] - yl[0]);
    return R_NilValue;
}

SEXP qt_qsetSceneRect_QGraphicsView(SEXP s, SEXP xlim, SEXP ylim)
{
    double *xl = REAL(xlim), *yl = REAL(ylim);
    unwrapQObject(s, QGraphicsView)->setSceneRect(xl[0], yl[0], xl[1] - xl[0], yl[1] - yl[0]);
    return R_NilValue;
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


SEXP qt_setParentItem(SEXP item, SEXP parent)
{
    // FIXME error: QGraphicsItem is not a QObject
    // unwrapQObject(item, QGraphicsItem)->setParentItem(unwrapQObject(parent, QGraphicsItem));
    return R_NilValue;
}

SEXP qt_setPos_QGraphicsItem(SEXP item, SEXP x, SEXP y)
{
    // unwrapQObject(item, QGraphicsItem)->setPos(asReal(x), asReal(y));
    return R_NilValue;
}

SEXP qt_setZValue(SEXP item, SEXP z)
{
    // unwrapQObject(item, QGraphicsItem)->setZValue(asReal(z));
    return R_NilValue;
}

SEXP qt_setToolTip_QGraphicsItem(SEXP item, SEXP s)
{
    // unwrapQObject(item, QGraphicsItem)->setToolTip(sexp2qstring(s));
    return R_NilValue;
}

SEXP qt_qgraphicsProxyWidget(SEXP w)
{
    QGraphicsProxyWidget *pw = new QGraphicsProxyWidget(0, 0);
    pw->setWidget(unwrapQObject(w, QWidget));
    return wrapQObject(pw);
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
    SEXP ans;
    QGraphicsScene *scene = unwrapQObject(rscene, QGraphicsScene);
    return wrapQWidget(new QGraphicsView(scene, 0));
}

SEXP
qt_fitScene_QGraphicsView(SEXP v) {
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
	    textitem = (QGraphicsTextItem *) ilist[i];
	    if (textitem)
		textitem->setTextInteractionFlags(Qt::NoTextInteraction);
	}
    }
    else if (smode == "editor") {
	for (int i = 0; i < ilist.size(); ++i) {
	    textitem = (QGraphicsTextItem *) ilist[i];
	    if (textitem)
		textitem->setTextInteractionFlags(Qt::TextEditorInteraction);
	}
    }
    else if (smode == "browser") {
	for (int i = 0; i < ilist.size(); ++i) {
	    textitem = (QGraphicsTextItem *) ilist[i];
	    if (textitem)
		textitem->setTextInteractionFlags(Qt::TextBrowserInteraction);
	}
    }
    return R_NilValue;
}

