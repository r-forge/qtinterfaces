#ifndef QTGUI_GRAPHICSVIEW_H
#define QTGUI_GRAPHICSVIEW_H


SEXP qt_qgraphicsScene();
SEXP qt_qaddItem_QGraphicsScene(SEXP s, SEXP item);
SEXP qt_qsceneRect_QGraphicsScene(SEXP x);
SEXP qt_qsceneRect_QGraphicsView(SEXP x);
SEXP qt_qsetSceneRect_QGraphicsScene(SEXP s, SEXP xlim, SEXP ylim);
SEXP qt_qsetSceneRect_QGraphicsView(SEXP s, SEXP xlim, SEXP ylim);
SEXP qt_qclear_QGraphicsScene(SEXP s);
SEXP qt_qclearSelection_QGraphicsScene(SEXP s);
SEXP qt_qitemsBoundingRect(SEXP s);
SEXP qt_qsetFont_QGraphicsScene(SEXP s, SEXP font);
SEXP qt_qsetItemIndexMethod(SEXP s, SEXP method);
SEXP qt_qsetBackgroundBrush(SEXP s, SEXP brush);
SEXP qt_setParentItem(SEXP item, SEXP parent);
SEXP qt_setPos_QGraphicsItem(SEXP item, SEXP x, SEXP y);
SEXP qt_setZValue(SEXP item, SEXP z);
SEXP qt_setToolTip_QGraphicsItem(SEXP item, SEXP s);
SEXP qt_qgraphicsProxyWidget(SEXP w);
SEXP scene_addPoints(SEXP scene, SEXP x, SEXP y, SEXP radius);
SEXP scene_addLines(SEXP scene, SEXP x, SEXP y, SEXP lwd);
SEXP scene_addSegments(SEXP scene, SEXP x1, SEXP y1, SEXP x2, SEXP y2, SEXP lwd);
SEXP scene_addRect(SEXP scene, SEXP x, SEXP y, SEXP w, SEXP h);
SEXP scene_addText(SEXP scene, SEXP x, SEXP y, SEXP labels, SEXP html);
SEXP qt_qgraphicsView(SEXP rscene);
SEXP qt_fitScene_QGraphicsView(SEXP v);
SEXP qt_qsetTransform_QGraphicsView(SEXP v,
				    SEXP xscale,
				    SEXP yscale,
				    SEXP rotate,
				    SEXP translate);
SEXP qt_qsetDragMode(SEXP v, SEXP mode);
SEXP view_setAntialias(SEXP v, SEXP mode);


#endif
