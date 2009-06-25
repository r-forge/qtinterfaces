
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

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
    CALLDEF(qt_qsetTextButton, 2),
    CALLDEF(qt_qtextButton, 1),
    CALLDEF(qt_qlabel, 1),
    CALLDEF(qt_qsetTextLabel, 2),
    CALLDEF(qt_qtextLabel, 1),
    CALLDEF(qt_qlineEdit, 1),
    CALLDEF(qt_qsetTextLineEdit, 2),
    CALLDEF(qt_qtextLineEdit, 1),
    CALLDEF(qt_qcheckBox, 1),
    CALLDEF(qt_qisCheckedButton, 1),
    CALLDEF(qt_qsetCheckedButton, 2),

    CALLDEF(qt_qtabWidget, 0),
    CALLDEF(qt_qaddTab, 4),
    CALLDEF(qt_qremoveTab, 2),
    CALLDEF(qt_qstackedWidget, 0),
    CALLDEF(qt_qaddWidgetToStack, 3), 
    CALLDEF(qt_qremoveWidgetFromStack, 2),
    CALLDEF(qt_qcurrentIndex, 1),
    CALLDEF(qt_qsetCurrentIndex, 2),

    CALLDEF(qt_qlayout, 1),
    CALLDEF(qt_qsetLayout, 2),
    CALLDEF(qt_qsetContentsMarginsLayout, 5),
    CALLDEF(qt_qsetSpacing, 2),
    CALLDEF(qt_qsetVerticalSpacing, 2),
    CALLDEF(qt_qsetHorizontalSpacing, 2),
    CALLDEF(qt_qaddWidgetToLayout, 6),
    CALLDEF(qt_qremoveWidgetFromLayout, 2),
    CALLDEF(qt_qaddLayoutToLayout, 6),
    CALLDEF(qt_qcolumnCount, 1),
    CALLDEF(qt_qrowCount, 1),

    CALLDEF(qt_qshow, 1),
    CALLDEF(qt_qupdate, 1),
    CALLDEF(qt_qclose, 1),
    CALLDEF(qt_qraise, 1),
    CALLDEF(qt_qlower, 1),
    CALLDEF(qt_qparent, 1),
    CALLDEF(qt_qsetParent, 2),
    CALLDEF(qt_qresize, 3),
    CALLDEF(qt_qheight, 1),
    CALLDEF(qt_qwidth, 1),
    CALLDEF(qt_qsetMinimumSize, 3),
    CALLDEF(qt_setExpanding, 3),
    CALLDEF(qt_qsetContentsMargins, 5),
    CALLDEF(qt_qisEnabled, 1),
    CALLDEF(qt_qsetEnabled, 2),
    CALLDEF(qt_qwindowTitle, 1),
    CALLDEF(qt_qsetWindowTitle, 2),
    CALLDEF(qt_qrender, 2),
    CALLDEF(qt_qrenderGraphicsView, 1),
    CALLDEF(qt_qrenderToPixmap, 2),
    CALLDEF(qt_qrenderToSVG, 2),

    CALLDEF(qt_qgraphicsScene, 0),
    CALLDEF(qt_qaddItem_QGraphicsScene, 2),
    CALLDEF(qt_qsceneRect_QGraphicsScene, 1),
    CALLDEF(qt_qsceneRect_QGraphicsView, 1),
    CALLDEF(qt_qsetSceneRect_QGraphicsScene, 3),
    CALLDEF(qt_qsetSceneRect_QGraphicsView, 3),
    CALLDEF(qt_qclear_QGraphicsScene, 1),
    CALLDEF(qt_qclearSelection_QGraphicsScene, 1),
    CALLDEF(qt_qitemsBoundingRect, 1),
    CALLDEF(qt_qsetFont_QGraphicsScene, 2),
    CALLDEF(qt_qsetItemIndexMethod, 2),
    CALLDEF(qt_qsetBackgroundBrush, 2),
    CALLDEF(qt_setParentItem, 2),
    CALLDEF(qt_setPos_QGraphicsItem, 3),
    CALLDEF(qt_setZValue, 2),
    CALLDEF(qt_setToolTip_QGraphicsItem, 2),
    CALLDEF(qt_qgraphicsProxyWidget, 1),
    CALLDEF(scene_addPoints, 4),
    CALLDEF(scene_addLines, 4),
    CALLDEF(scene_addSegments, 6),
    CALLDEF(scene_addRect, 5),
    CALLDEF(scene_addText, 5),
    CALLDEF(qt_qgraphicsView, 1),
    CALLDEF(qt_fitScene_QGraphicsView, 1),
    CALLDEF(qt_qsetTransform_QGraphicsView, 5),
    CALLDEF(qt_qsetDragMode, 2),
    CALLDEF(view_setAntialias, 2),

    {NULL, NULL, 0}
};


void R_init_qtgui(DllInfo *dll)
{
    // Register C routines
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

