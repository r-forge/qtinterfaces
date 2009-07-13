#ifndef QTGUI_GRAPHICSVIEW_H
#define QTGUI_GRAPHICSVIEW_H


SEXP qt_qgraphicsScene();
SEXP qt_qaddItem_QGraphicsScene(SEXP s, SEXP item);
SEXP qt_qsceneRect_QGraphicsScene(SEXP x);
SEXP qt_qsceneRect_QGraphicsView(SEXP x);
SEXP qt_qsetSceneRect_QGraphicsScene(SEXP s, SEXP rrect);
SEXP qt_qsetSceneRect_QGraphicsView(SEXP s, SEXP rrect);
SEXP qt_qclear_QGraphicsScene(SEXP s);
SEXP qt_qclearSelection_QGraphicsScene(SEXP s);
SEXP qt_qitemsBoundingRect(SEXP s);
SEXP qt_qsetFont_QGraphicsScene(SEXP s, SEXP font);
SEXP qt_qsetItemIndexMethod(SEXP s, SEXP method);
SEXP qt_qsetBackgroundBrush(SEXP s, SEXP brush);
SEXP qt_qsetParentItem(SEXP item, SEXP parent);
SEXP qt_qsetPos_QGraphicsItem(SEXP item, SEXP rpoint);
SEXP qt_qsetZValue(SEXP item, SEXP z);
SEXP qt_qsetToolTip_QGraphicsItem(SEXP item, SEXP s);
SEXP qt_qgraphicsEllipseItem(SEXP x, SEXP y, SEXP width, SEXP height, SEXP pars);
SEXP qt_qgraphicsLineItem(SEXP x1, SEXP y1, SEXP x2, SEXP y2, SEXP pars);
SEXP qt_qtext_QGraphicsItem(SEXP item);
SEXP qt_qsetText_QGraphicsItem(SEXP item, SEXP label);
SEXP qt_qgraphicsProxyWidget(SEXP w);
SEXP scene_addPoints(SEXP scene, SEXP x, SEXP y, SEXP radius);
SEXP scene_addLines(SEXP scene, SEXP x, SEXP y, SEXP lwd);
SEXP scene_addSegments(SEXP scene, SEXP x1, SEXP y1, SEXP x2, SEXP y2, SEXP lwd);
SEXP scene_addRect(SEXP scene, SEXP x, SEXP y, SEXP w, SEXP h);
SEXP scene_addText(SEXP scene, SEXP x, SEXP y, SEXP labels, SEXP html);
SEXP qt_qgraphicsView(SEXP rscene);
SEXP qt_qfitScene_QGraphicsView(SEXP v);
SEXP qt_qsetTransform_QGraphicsView(SEXP v,
				    SEXP xscale,
				    SEXP yscale,
				    SEXP rotate,
				    SEXP translate);
SEXP qt_qsetDragMode(SEXP v, SEXP mode);
SEXP view_setAntialias(SEXP v, SEXP mode);
SEXP qt_qsetItemFlags(SEXP x, SEXP flag, SEXP status);
SEXP qt_qsetTextItemInteraction(SEXP x, SEXP mode);

SEXP qt_qboundingRect_QGraphicsItem(SEXP rself);
SEXP qt_qitemsAtPoint_QGraphicsScene(SEXP rself, SEXP rpoint);
SEXP qt_qitemsInRect_QGraphicsScene(SEXP rself, SEXP rrect);
SEXP qt_qsetGeometry_QGraphicsWidget(SEXP rself, SEXP rx);
SEXP qt_qgeometry_QGraphicsWidget(SEXP rself);
SEXP qt_qmatrix_QGraphicsView(SEXP extp, SEXP inverted);
SEXP qt_qmatrix_QGraphicsItem(SEXP rself, SEXP rinverted);
SEXP qt_qsetMatrix_QGraphicsView(SEXP extp, SEXP rmatrix);
SEXP qt_qupdate_QGraphicsView(SEXP rself);
SEXP qt_qupdate_QGraphicsItem(SEXP rself);
SEXP qt_qupdate_QGraphicsScene(SEXP rself);
SEXP qt_qcacheMode_QGraphicsItem(SEXP rself);
SEXP qt_qsetCacheMode_QGraphicsItem(SEXP rself, SEXP rmode);
SEXP qt_qsetFocus_QGraphicsItem(SEXP rself);
SEXP qt_qcolStretch_QGraphicsGridLayout(SEXP rself);
SEXP qt_qrowStretch_QGraphicsGridLayout(SEXP rself);
SEXP qt_qsetColStretch_QGraphicsGridLayout(SEXP rself, SEXP rstretch);
SEXP qt_qsetRowStretch_QGraphicsGridLayout(SEXP rself, SEXP rstretch);
SEXP qt_qsetHorizontalSpacing_QGraphicsGridLayout(SEXP rself, SEXP rspacing);
SEXP qt_qsetVerticalSpacing_QGraphicsGridLayout(SEXP rself, SEXP rspacing);
SEXP qt_qsetMinimumSize_QGraphicsLayoutItem(SEXP ritem, SEXP rsize);
SEXP qt_qminimumSize_QGraphicsLayoutItem(SEXP ritem);

#endif
