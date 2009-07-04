
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

#include "attributes.h"
#include "actions.h"
#include "basic.h"
#include "containers.h"
#include "layout.h"
#include "qwidgetWrappers.h"
#include "graphicsView.h"

void R_init_qtgui(DllInfo *dll);

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static R_CallMethodDef CallEntries[] = {

    CALLDEF(qt_qwidget, 0),
    CALLDEF(qt_qpushButton, 1),
    CALLDEF(qt_qsetTextButton, 2), //
    CALLDEF(qt_qtextButton, 1), //
    CALLDEF(qt_qlabel, 1),
    CALLDEF(qt_qsetTextLabel, 2), //
    CALLDEF(qt_qtextLabel, 1), //
    CALLDEF(qt_qlineEdit, 1),
    CALLDEF(qt_qsetTextLineEdit, 2), //
    CALLDEF(qt_qtextLineEdit, 1), //
    CALLDEF(qt_qcheckBox, 1),
    CALLDEF(qt_qisCheckedButton, 1), //
    CALLDEF(qt_qsetCheckedButton, 2), //

    CALLDEF(qt_qtabWidget, 0),
    CALLDEF(qt_qaddTab, 4),
    CALLDEF(qt_qremoveTab, 2),
    CALLDEF(qt_qstackedWidget, 0),
    CALLDEF(qt_qaddWidgetToStack, 3), 
    CALLDEF(qt_qremoveWidgetFromStack, 2),
    CALLDEF(qt_qcurrentIndex, 1), //
    CALLDEF(qt_qsetCurrentIndex, 2), //

    CALLDEF(qt_qlayout, 1),
    CALLDEF(qt_qsetLayout, 2),
    CALLDEF(qt_qsetContentsMarginsLayout, 5),
    CALLDEF(qt_qsetSpacing, 2), //
    CALLDEF(qt_qsetVerticalSpacing, 2), //
    CALLDEF(qt_qsetHorizontalSpacing, 2), //
    CALLDEF(qt_qaddWidgetToLayout, 6),
    CALLDEF(qt_qremoveWidgetFromLayout, 2),
    CALLDEF(qt_qaddLayoutToLayout, 6),
    CALLDEF(qt_qcolumnCount, 1),
    CALLDEF(qt_qrowCount, 1),

    CALLDEF(qt_qshow, 1), //
    CALLDEF(qt_qupdate_QWidget, 1), //
    CALLDEF(qt_qclose, 1), //
    CALLDEF(qt_qraise, 1), //
    CALLDEF(qt_qlower, 1), //
    CALLDEF(qt_qparent, 1),
    CALLDEF(qt_qsetParent, 2),
    CALLDEF(qt_qresize, 3), //
    CALLDEF(qt_qheight, 1), //
    CALLDEF(qt_qwidth, 1), //
    CALLDEF(qt_qsetMinimumSize, 3), //
    CALLDEF(qt_setExpanding, 3), // QSizePolicy
    CALLDEF(qt_qsetContentsMargins, 5),
    CALLDEF(qt_qisEnabled, 1), //
    CALLDEF(qt_qsetEnabled, 2), //
    CALLDEF(qt_qwindowTitle, 1), //
    CALLDEF(qt_qsetWindowTitle, 2), //
    CALLDEF(qt_qrender, 2),
    CALLDEF(qt_qrenderGraphicsView, 1),
    CALLDEF(qt_qrenderToPixmap, 2),
    CALLDEF(qt_qrenderToSVG, 2),

    CALLDEF(qt_qaction, 5),
    CALLDEF(qt_qaddActionToQWidget, 2),
    CALLDEF(qt_qaddActionToQMenu, 2),
    CALLDEF(qt_qaddAction_QGraphicsWidget, 2),
    CALLDEF(qt_qsetContextMenuPolicy, 2),
    CALLDEF(qt_qgraphicsScene, 0),
    CALLDEF(qt_qaddItem_QGraphicsScene, 2),
    CALLDEF(qt_qsceneRect_QGraphicsScene, 1), //
    CALLDEF(qt_qsceneRect_QGraphicsView, 1), //
    CALLDEF(qt_qboundingRect_QGraphicsItem, 1),
    CALLDEF(qt_qsetSceneRect_QGraphicsScene, 3), //
    CALLDEF(qt_qsetSceneRect_QGraphicsView, 3), //
    CALLDEF(qt_qclear_QGraphicsScene, 1), //
    CALLDEF(qt_qclearSelection_QGraphicsScene, 1), //
    CALLDEF(qt_qitemsBoundingRect, 1),
    CALLDEF(qt_qitemsAtPoint_QGraphicsScene, 2),
    CALLDEF(qt_qitemsInRect_QGraphicsScene, 2),
    CALLDEF(qt_qsetFont_QGraphicsScene, 2), // QFont
    CALLDEF(qt_qsetItemIndexMethod, 2), // ItemIndexMethod (no string support)
    CALLDEF(qt_qsetBackgroundBrush, 2), // QBrush
    CALLDEF(qt_qsetParentItem, 2),
    CALLDEF(qt_qsetPos_QGraphicsItem, 3),
    CALLDEF(qt_qsetGeometry_QGraphicsWidget, 2),
    CALLDEF(qt_qgeometry_QGraphicsWidget, 1),
    CALLDEF(qt_qsetZValue, 2),
    CALLDEF(qt_qsetToolTip_QGraphicsItem, 2),
    CALLDEF(qt_qgraphicsEllipseItem, 5),
    CALLDEF(qt_qgraphicsLineItem, 5),
    CALLDEF(qt_qgraphicsProxyWidget, 1),
    CALLDEF(scene_addPoints, 4),
    CALLDEF(scene_addLines, 4),
    CALLDEF(scene_addSegments, 6),
    CALLDEF(scene_addRect, 5),
    CALLDEF(scene_addText, 5),
    CALLDEF(qt_qgraphicsView, 1),
    CALLDEF(qt_qfitScene_QGraphicsView, 1),
    CALLDEF(qt_qsetTransform_QGraphicsView, 5),
    CALLDEF(qt_qmatrix_QGraphicsItem, 2),
    CALLDEF(qt_qmatrix_QGraphicsView, 2),
    CALLDEF(qt_qsetMatrix_QGraphicsView, 2),
    CALLDEF(qt_qupdate_QGraphicsView, 1),
    CALLDEF(qt_qupdate_QGraphicsScene, 1), //
    CALLDEF(qt_qupdate_QGraphicsItem, 1),
    CALLDEF(qt_qsetDragMode, 2), // DragMode
    CALLDEF(view_setAntialias, 2), // RenderHints
    CALLDEF(qt_qsetItemFlags, 3),
    CALLDEF(qt_qsetTextItemInteraction, 2),
    CALLDEF(qt_qsetCacheMode_QGraphicsItem, 2),
    CALLDEF(qt_qcacheMode_QGraphicsItem, 1),
    CALLDEF(qt_qsetFocus_QGraphicsItem, 1),

    CALLDEF(qt_qrowStretch_QGraphicsGridLayout, 1),
    CALLDEF(qt_qcolStretch_QGraphicsGridLayout, 1),
    CALLDEF(qt_qsetRowStretch_QGraphicsGridLayout, 2),
    CALLDEF(qt_qsetColStretch_QGraphicsGridLayout, 2),
    CALLDEF(qt_qsetHorizontalSpacing_QGraphicsGridLayout, 2),
    CALLDEF(qt_qsetVerticalSpacing_QGraphicsGridLayout, 2),
    
    CALLDEF(qt_qsetDeleteOnClose, 2),

    {NULL, NULL, 0}
};


void R_init_qtgui(DllInfo *dll)
{
    // Register C routines
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

