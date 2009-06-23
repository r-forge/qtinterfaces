
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsTextItem>

#include <qtbase.h>


#include <R_ext/GraphicsEngine.h>
#include <R_ext/GraphicsDevice.h>
#include <R_ext/Error.h>


#include "scenedevice.hpp"

typedef Rboolean
(*RSceneDeviceCreateFun)(pDevDesc,
			 double width, double height,
			 double ps,
			 RSceneDevice *qdev);


static GEDevDesc*
createRSceneDevice(double width, double height,
		   double ps,
		   RSceneDevice *qdev,
		   RSceneDeviceCreateFun init_fun);

static
RSceneDevice *startdev(double width,
		       double height,
		       double pointsize,
		       const char *family);


static void
do_QTScene(double width,
	   double height,
	   double pointsize,
	   RSceneDevice *qdev);









SEXP
qt_qsceneDevice(SEXP width,
		SEXP height,
		SEXP pointsize,
		SEXP family)
{
    return wrapQObject(startdev(asReal(width), 
				asReal(height), 
				asReal(pointsize), 
				CHAR(asChar(family))));
}

SEXP
qt_qsceneView(SEXP scene)
{
    QGraphicsView *v = new QGraphicsView(unwrapQObject(scene, QGraphicsScene), 0);
    v->setRenderHints(QPainter::TextAntialiasing);
    v->setInteractive(true);
    // v->setDragMode(QGraphicsView::RubberBandDrag);
    v->setDragMode(QGraphicsView::ScrollHandDrag);
    return wrapQWidget(v);
}

static
RSceneDevice *startdev(double width,
		       double height,
		       double pointsize,
		       const char *family)
{
    RSceneDevice *dev = new RSceneDevice(width * 72, height * 72, 
					 pointsize, family);
    return dev;
}


static void
do_QTScene(double width,
	   double height,
	   double pointsize,
	   RSceneDevice *qdev)
{
    if (width <= 0 || height <= 0) {
	error("Invalid width or height in do_QTScene: (%g, %g)", width, height);
    }
    createRSceneDevice(width, height, pointsize, 
		       qdev, RSceneDeviceDriver);
    return;
}






RSceneDevice::RSceneDevice(double width, double height,
			   double pointsize, 
			   const char *family)
    : QGraphicsScene(0)
{
    debug = false;
    default_family = QString(family);
    setDeviceNumber(curDevice());
    setSceneRect(0.0, 0.0, width, height);
    do_QTScene(width, height, pointsize, this);
}

// typedef struct {
//     /*
//      * Colours
//      *
//      * NOTE:  Alpha transparency included in col & fill
//      */
//     int col;             /* pen colour (lines, text, borders, ...) */
//     int fill;            /* fill colour (for polygons, circles, rects, ...) */
//     double gamma;        /* Gamma correction */
//     /*
//      * Line characteristics
//      */
//     double lwd;          /* Line width (roughly number of pixels) */
//     int lty;             /* Line type (solid, dashed, dotted, ...) */
//     R_GE_lineend lend;   /* Line end */
//     R_GE_linejoin ljoin; /* line join */
//     double lmitre;       /* line mitre */
//     /*
//      * Text characteristics
//      */
//     double cex;          /* Character expansion (font size = fontsize*cex) */
//     double ps;           /* Font size in points */
//     double lineheight;   /* Line height (multiply by font size) */
//     int fontface;        /* Font face (plain, italic, bold, ...) */
//     char fontfamily[201]; /* Font family */
// } R_GE_gcontext;




// device 'callbacks'



void 
RSceneDevice::Circle(double x, double y, double r,
		     R_GE_gcontext *gc)
{
    if (debug) Rprintf("RSceneDevice::Circle\n");
    QPen pen(Qt::NoPen);
    QBrush brush;
    QRectF rect(x - r, y - r, 2 * r, 2 * r);
    // painter.setClipRect(clip_rect); // FIXME: no clipping!
    int col = gc->col;
    int fill = gc->fill;
    // double gamma = gc->gamma;
    double lwd = gc->lwd;
    int lty = gc->lty;     // FIXME: should be easy to add
    if (col != NA_INTEGER && !R_TRANSPARENT(col)) {
	pen.setStyle(lty2style(lty));
	pen.setColor(r2qColor(col));
	pen.setWidthF(lwd);
    }
    if (fill != NA_INTEGER && !R_TRANSPARENT(fill)) {
	brush.setColor(r2qColor(fill));
	brush.setStyle(Qt::SolidPattern);
    }
    addEllipse(rect, pen, brush);
    return;
}

void 
RSceneDevice::Line(double x1, double y1, double x2, double y2,
		   R_GE_gcontext *gc)
{
    if (debug) Rprintf("RSceneDevice::Line\n");
    QPen pen(Qt::NoPen);
    QLineF line(x1, y1, x2, y2);
    // painter.setClipRect(clip_rect);
    int col = gc->col;
    // double gamma = gc->gamma;
    double lwd = gc->lwd;
    int lty = gc->lty;
    if (col != NA_INTEGER && !R_TRANSPARENT(col)) {
	pen.setStyle(lty2style(lty));
	pen.setColor(r2qColor(col));
	pen.setWidthF(lwd);
    }
    addLine(line, pen);
    return;
}


void 
RSceneDevice::Polygon(int n, double *x, double *y,
		      R_GE_gcontext *gc)
{
    if (debug) Rprintf("RSceneDevice::Polygon\n");
    QPen pen(Qt::NoPen);
    QBrush brush;
    QPolygonF polygon;
    // painter.setClipRect(clip_rect);
    if (n > 0) {
	for (int i = 0; i < n; i++) {
	    polygon <<  QPointF(x[i], y[i]);
	}
	// FIXME: do we need to explicitly close the polygon?
	polygon <<  QPointF(x[0], y[0]);
	int col = gc->col;
	int fill = gc->fill;
	//double gamma = gc->gamma;
	double lwd = gc->lwd;
	int lty = gc->lty;
	if (col != NA_INTEGER && !R_TRANSPARENT(col)) {
	    pen.setStyle(lty2style(lty));
	    pen.setColor(r2qColor(col));
	    pen.setWidthF(lwd);
	}
	if (fill != NA_INTEGER && !R_TRANSPARENT(fill)) {
	    brush.setColor(r2qColor(fill));
	    brush.setStyle(Qt::SolidPattern);
	}
	addPolygon(polygon, pen, brush);
     }
    return;
}


void 
RSceneDevice::Polyline(int n, double *x, double *y,
			R_GE_gcontext *gc)
{
    if (debug) Rprintf("RSceneDevice::Polyline\n");
    QPen pen(Qt::NoPen);
    // painter.setClipRect(clip_rect);
    int col = gc->col;
    // double gamma = gc->gamma;
    double lwd = gc->lwd;
    int lty = gc->lty;
    //     R_GE_lineend lend;   /* Line end */
    //     R_GE_linejoin ljoin; /* line join */
    //     double lmitre;       /* line mitre */
    if (col != NA_INTEGER && !R_TRANSPARENT(col)) {
	pen.setStyle(lty2style(lty));
	pen.setColor(r2qColor(col));
	pen.setWidthF(lwd);
    }
    if (n > 1) {
	for (int i = 1; i < n; i++) {
	    addLine(x[i-1], y[i-1], x[i], y[i], pen);
	}
    }
    return;
}


void 
RSceneDevice::Rect(double x0, double y0, double x1, double y1,
		   R_GE_gcontext *gc)
{
    if (debug) Rprintf("RSceneDevice::Rect\n");
    QPen pen(Qt::NoPen);
    QBrush brush;
    QRectF rect(x0, y0, x1 - x0, y1 - y0);
    // painter.setClipRect(clip_rect);
    int col = gc->col;
    int fill = gc->fill;
    //double gamma = gc->gamma;
    double lwd = gc->lwd;
    int lty = gc->lty;
    if (col != NA_INTEGER && !R_TRANSPARENT(col)) {
	pen.setStyle(lty2style(lty));
	pen.setColor(r2qColor(col));
	pen.setWidthF(lwd);
    }
    if (fill != NA_INTEGER && !R_TRANSPARENT(fill)) {
	brush.setColor(r2qColor(fill));
	brush.setStyle(Qt::SolidPattern);
    }
    addRect(rect, pen, brush);
    return;
}


void 
RSceneDevice::TextUTF8(double x, double y, char *str,
		       double rot, double hadj,
		       R_GE_gcontext *gc)
{
    if (debug) Rprintf("RSceneDevice::TextUTF8\n");
    int col = gc->col;
    //double gamma = gc->gamma;
    double cex = gc->cex;
    double ps = gc->ps;
    double lineheight = gc->lineheight;
    int fontface = gc->fontface;
    char* fontfamily = gc->fontfamily; // [201] ??
    QString qstr = QString::fromUtf8(str);
    QGraphicsTextItem 
	*text = addText(qstr, 
			r2qFont(fontfamily, fontface, ps, cex, lineheight,
				defaultFamily()));
    text->setDefaultTextColor(r2qColor(col));
    text->setTextInteractionFlags(Qt::TextEditorInteraction);
    QRectF brect = text->boundingRect();
    text->rotate(-rot);
    text->translate(-hadj * brect.width(), -0.7 * brect.height());
    text->setPos(x, y);
    return;
}



// void R_PreserveObject(SEXP object)
// void R_ReleaseObject(SEXP object)


void 
RSceneDevice::NewPage(R_GE_gcontext *gc)
{
    if (debug) Rprintf("RSceneDevice::NewPage\n");
    clear();
    QApplication::processEvents();
    return;
    //     // FIXME: deal with par("bg")? 
    //     int fill = gc->fill;
    //     if (fill != NA_INTEGER && !R_TRANSPARENT(fill)) {
    // 	// painter.fillRect(painter.viewport(), r2qColor(fill));
    // 	current_device->getPixmap()->fill(r2qColor(fill));
    //     } else {
    // 	current_device->getPixmap()->fill(Qt::transparent); // Qt::transparent
    //     }
}


// FIXME: doesn't work for some plotmath things at least (e.g. \sum)
void 
RSceneDevice::MetricInfo(int c,
			 R_GE_gcontext *gc,
			 double* ascent, double* descent,
			 double* width)
{
    if (debug) Rprintf("RSceneDevice::MetricInfo\n");
    double cex = gc->cex;
    double ps = gc->ps;
    double lineheight = gc->lineheight;
    int fontface = gc->fontface;
    char* fontfamily = gc->fontfamily;
    bool Unicode = mbcslocale; // && (gc->fontface != 5);
    if (c < 0) { Unicode = TRUE; c = -c; }
    QFontMetrics fm(r2qFont(fontfamily, fontface, ps, cex, lineheight, "Helvetica"));
    QChar uc = QChar(c);
    QRect bb = fm.boundingRect(uc); 
    // Note: bb.bottom() is probably negative. 
    // WAS:
    //     *ascent = (double) (bb.height() + bb.bottom());
    //     *descent = (double) (-bb.bottom());
    *ascent = (double) (bb.height());
    *descent = (double) (0.0);
    *width = (double) fm.width(uc);
    return;
}


double 
RSceneDevice::StrWidthUTF8(char *str,
			   R_GE_gcontext *gc)
{
    if (debug) Rprintf("RSceneDevice::StrWidthUTF8\n");
    double cex = gc->cex;
    double ps = gc->ps;
    double lineheight = gc->lineheight;
    int fontface = gc->fontface;
    char* fontfamily = gc->fontfamily;
    QString qstr = QString::fromUtf8(str);
    QGraphicsTextItem *text = new QGraphicsTextItem(str, 0);
    text->setFont(r2qFont(fontfamily, fontface, 
			  ps, cex, lineheight,
			  defaultFamily()));
    QRectF brect = text->boundingRect();
    delete text;
    return brect.width();
}


#define asSceneDevice(x) ((RSceneDevice *) x->deviceSpecific)
// alternative: use qobject_cast ?



static void 
QT_Circle(double x, double y, double r,
	  R_GE_gcontext *gc, pDevDesc dev)
{
    return asSceneDevice(dev)->Circle(x, y, r, gc);
}

static void 
QT_Line(double x1, double y1, double x2, double y2,
	R_GE_gcontext *gc,
	pDevDesc dev)
{
    return asSceneDevice(dev)->Line(x1, y1, x2, y2, gc);
}

static void 
QT_Polygon(int n, double *x, double *y,
	   R_GE_gcontext *gc,
	   pDevDesc dev)
{
    return asSceneDevice(dev)->Polygon(n, x, y, gc);
}

static void 
QT_Polyline(int n, double *x, double *y,
	    R_GE_gcontext *gc,
	    pDevDesc dev)
{
    return asSceneDevice(dev)->Polyline(n, x, y, gc);
}

static void 
QT_Rect(double x0, double y0, double x1, double y1,
	R_GE_gcontext *gc,
	pDevDesc dev)
{
    return asSceneDevice(dev)->Rect(x0, y0, x1, y1, gc);
}

static void 
QT_TextUTF8(double x, double y, char *str,
	    double rot, double hadj,
	    R_GE_gcontext *gc,
	    pDevDesc dev)
{
    return asSceneDevice(dev)->TextUTF8(x, y, str, rot, hadj, gc);
}

static void 
QT_NewPage(R_GE_gcontext *gc,
	   pDevDesc dev)
{
    return asSceneDevice(dev)->NewPage(gc);
}

static void 
QT_MetricInfo(int c, R_GE_gcontext *gc,
	      double* ascent, double* descent,
	      double* width,
	      pDevDesc dev)
{
    return asSceneDevice(dev)->MetricInfo(c, gc, ascent, descent, width);
}

static double 
QT_StrWidthUTF8(char *str, R_GE_gcontext *gc,
		pDevDesc dev)
{
    return asSceneDevice(dev)->StrWidthUTF8(str, gc);
}






static void QT_Close(pDevDesc dev)
{
    return;
}


static void QT_Activate(pDevDesc dev)
{
    return;
}

static void QT_Clip(double left, 
		    double right, 
		    double bottom, 
		    double top,
		    pDevDesc dev)
{
    dev->clipLeft   = left;
    dev->clipRight  = right;
    dev->clipBottom = bottom;
    dev->clipTop    = top;
    return;
}

static void QT_Deactivate(pDevDesc dev)
{
    return;
}

static void QT_Mode(int mode, pDevDesc dev)
{
    Q_UNUSED(mode);
    Q_UNUSED(dev);
    return;
}


static Rboolean QT_Locator(double *x, double *y, pDevDesc dev)
{
    return FALSE;
}


// FIXME: not OK
static void QT_Size(double *left, double *right,
		    double *bottom, double *top,
		    pDevDesc dev)
{
    // FIXME FIXME FIXME : should I do this here?
    dev->left = 0;
    dev->right = asSceneDevice(dev)->width();
    dev->bottom = asSceneDevice(dev)->height();
    dev->top = 0;
    // Rprintf("Someone called QT_Size\n");
    *left = 0.0;
    *right = asSceneDevice(dev)->width();
    *bottom = asSceneDevice(dev)->height();
    *top = 0.0;
    return;
}








/* Device driver entry point */
Rboolean
RSceneDeviceDriver(pDevDesc dev,
		   double width, double height,
		   double ps,
		   RSceneDevice *qdev)
{
    // store a pointer to the device object (in Qt)
    dev->deviceSpecific = (void *) qdev;

    // pointsize?

    /*
     * Initial graphical settings
     */
    dev->startfont = 1;
    dev->startps = ps;
    dev->startcol = R_RGB(0, 0, 0);
    dev->startfill = R_TRANWHITE;
    dev->startlty = LTY_SOLID;
    dev->startgamma = 1;
    /*
     * Device physical characteristics
     */
    dev->left   = dev->clipLeft   = 0;
    dev->right  = dev->clipRight  = qdev->width();
    dev->bottom = dev->clipBottom = qdev->height();
    dev->top    = dev->clipTop    = 0;
    dev->cra[0] = 0.9 * ps;
    dev->cra[1] = 1.2 * ps;
    dev->xCharOffset = 0.4900;
    dev->yCharOffset = 0.3333;
    dev->yLineBias = 0.2;
    dev->ipr[0] = 1.0 / 72;
    dev->ipr[1] = 1.0 / 72;
    /*
     * Device capabilities
     */
    dev->canClip = FALSE;
    dev->canHAdj = 2;
    dev->canChangeGamma = FALSE;
    dev->displayListOn = TRUE;

    dev->hasTextUTF8 = TRUE;
    dev->textUTF8 = (void (*)()) QT_TextUTF8;
    dev->strWidthUTF8 = (double (*)()) QT_StrWidthUTF8;
    dev->wantSymbolUTF8 = TRUE;
    dev->useRotatedTextInContour = TRUE;

    /*
     * Mouse events
     */
//     dev->canGenMouseDown = TRUE;
//     dev->canGenMouseMove = TRUE;
//     dev->canGenMouseUp = TRUE; 
//     dev->canGenKeybd = TRUE;

    // gettingEvent; This is set while getGraphicsEvent is actively
    // looking for events

    /*
     * Device functions
     */
    dev->activate =    (void (*)()) QT_Activate;
    dev->circle =      (void (*)()) QT_Circle;
    dev->clip =        (void (*)()) QT_Clip;
    dev->close =       (void (*)()) QT_Close;
    dev->deactivate =  (void (*)()) QT_Deactivate;
    dev->locator = (Rboolean (*)()) QT_Locator;
    dev->line =        (void (*)()) QT_Line;
    dev->metricInfo =  (void (*)()) QT_MetricInfo;
    dev->mode =        (void (*)()) QT_Mode;
    dev->newPage =     (void (*)()) QT_NewPage;
    dev->polygon =     (void (*)()) QT_Polygon;
    dev->polyline =    (void (*)()) QT_Polyline;
    dev->rect =        (void (*)()) QT_Rect;
    dev->size =        (void (*)()) QT_Size;
    // dev->strWidth =  (double (*)()) QT_StrWidth;
    // dev->text =        (void (*)()) QT_Text;
    // dev->onexit =      (void (*)()) QT_OnExit; NULL is OK
    // dev->getEvent = SEXP (*getEvent)(SEXP, const char *);
    // dev->newFrameConfirm
    // dev->


    return TRUE;
}



static GEDevDesc*
createRSceneDevice(double width, double height,
		   double ps,
		   RSceneDevice *qdev,
		   RSceneDeviceCreateFun init_fun)
{
    pGEDevDesc gdd;
    // pDevDesc dev;

    R_GE_checkVersionOrDie(R_GE_version);
    R_CheckDeviceAvailable();
    BEGIN_SUSPEND_INTERRUPTS {
	pDevDesc dev;
	/* Allocate and initialize the device driver data */
	if (!(dev = (pDevDesc) calloc(1, sizeof(DevDesc))))
	    return 0; /* or error() */
	/* set up device driver or free 'dev' and error() */
	if (!init_fun(dev, width, height, ps, qdev)) {
	    free(dev); // delete qdev; // ??
	    error("unable to start device");
	}
	gdd = GEcreateDevDesc(dev);
	gdd->displayList = R_NilValue;
	// gdd->savedSnapshot = R_NilValue;
	GEaddDevice2(gdd, "QTScene");
    } END_SUSPEND_INTERRUPTS;

    return(gdd);
}





