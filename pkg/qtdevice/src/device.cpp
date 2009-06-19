
// FIXME: par("bg") is not honored

// Some Notes:

// 1. X11 refreshes the display after each piece is drawn.  Qt has two
// 1. functions that cause a refresh; update() and repaint().
// 1. update() does not cause an immediate update, usually it waits
// 1. till the event loop is idle (more precisely, till processEvents
// 1. is called).  repaint() causes an immediate refresh.  I have
// 1. implemented things such that all primitive operations cause an
// 1. update(), except _NewPage, which causes a repaint().  The effect
// 1. of update() is not immediate, and is apparent only when the R
// 1. event loop deigns to process Pending Qt events.  The repaint on
// 1. newpage forces a repaint, which avoids the possibility of a
// 1. particular page never being displayed at all if multiple pages
// 1. are created by a single command (for simple base graphics, they
// 1. are seen very briefly anyway, but grid is a bit more laid back).
// 1. Anyway, the point is that if you really want a refresh after
// 1. each step, change the update()s to repaint()s (this is not
// 1. necessary in practice because the event handlers are run
// 1. frequently enough)


#include <QApplication>
#include <QtGui>
#include <QWidget>
#include <QChar>
#include <QPoint>

#include <qtbase.h>

#include "device.hpp"


extern "C" {

SEXP do_newQtDevice(SEXP width, SEXP height, SEXP pointsize, SEXP family,
		    SEXP aaEdges, SEXP aaText, SEXP bgimage);

}



// global reference to current device, since I can't seem to find a
// way to retrieve it using CurrentDevice()

//QtRDevice *global_dev_ptr; // look for "// GLOBAL" to revert

#define global_dev_ptr ((QtRDevice *) ((pDevDesc) GEcurrentDevice())->deviceSpecific)



#define current_device ((QtRDevice *) dev->deviceSpecific)

#define clip_rect QRectF(dev->clipLeft, dev->clipBottom, dev->clipRight - dev->clipLeft, dev->clipTop - dev->clipBottom)

// #define clip_rect QRectF(dev->clipLeft, dev->clipTop, dev->clipRight - dev->clipLeft, dev->clipBottom - dev->clipTop)


static
QtRDevice *startdev(double width,
		    double height,
		    double pointsize,
		    const char *family,
		    bool edges_aa, bool text_aa,
		    const char *bgimage)
{
    QtRDevice *dev = new QtRDevice(width, height, pointsize, 
				   family, 
				   edges_aa, text_aa,
				   bgimage);
    dev->setCursor(Qt::CrossCursor);
    // GLOBAL    global_dev_ptr = dev;
    return dev;
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



static void QT_Circle(double x, double y, double r,
		      R_GE_gcontext *gc,
		      pDevDesc dev)
{
    QRectF rect(x - r, y - r, 2 * r, 2 * r); // FIXME: check
    QPainter painter(current_device->getPixmap());
    if (current_device->aaEdges()) painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRect(clip_rect);

    int col = gc->col;
    int fill = gc->fill;
    // double gamma = gc->gamma;
    double lwd = gc->lwd;
    int lty = gc->lty;     // FIXME: should be easy to add

    if (col != NA_INTEGER && !R_TRANSPARENT(col)) {
	QPen pen;
	pen.setStyle(lty2style(lty));
	pen.setColor(r2qColor(col));
	pen.setWidthF(lwd);
	painter.setPen(pen);
    }
    else {
	painter.setPen(QPen(Qt::NoPen));
    }
    if (fill != NA_INTEGER && !R_TRANSPARENT(fill)) {
	painter.setBrush(QBrush(r2qColor(fill)));
    }
    painter.drawEllipse(rect);
    current_device->update();
}

static void QT_Line(double x1, double y1, double x2, double y2,
		    R_GE_gcontext *gc,
		    pDevDesc dev)
{
    QLineF line(x1, y1, x2, y2);
    QPainter painter(current_device->getPixmap());
    if (current_device->aaEdges()) painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRect(clip_rect);

    int col = gc->col;
    // double gamma = gc->gamma;
    double lwd = gc->lwd;
    int lty = gc->lty;

    if (col != NA_INTEGER && !R_TRANSPARENT(col)) {
	QPen pen;
	pen.setStyle(lty2style(lty));
	pen.setColor(r2qColor(col));
	pen.setWidthF(lwd);
	painter.setPen(pen);
	painter.drawLine(line);
	current_device->update();
    }
}


static void QT_Polygon(int n, double *x, double *y,
		       R_GE_gcontext *gc,
		       pDevDesc dev)
{
    QPainter painter(current_device->getPixmap());
    painter.setClipRect(clip_rect);

    QPointF *points = (QPointF *) malloc(n * sizeof(QPointF));
    if (points != NULL) {
	for (int i = 0; i < n; i++) {
	    points[i] = QPointF(x[i], y[i]);
	}

	int col = gc->col;
	int fill = gc->fill;
	//double gamma = gc->gamma;
	double lwd = gc->lwd;
	int lty = gc->lty;

	QPen pen;
	if (col == NA_INTEGER || R_TRANSPARENT(col)) {
	    // disable anti-aliasing
	    painter.setRenderHint(QPainter::Antialiasing, false);
	    pen.setStyle(Qt::NoPen);
	}
	else {
	    if (current_device->aaEdges()) 
		painter.setRenderHint(QPainter::Antialiasing);
	    pen.setStyle(lty2style(lty));
	    pen.setColor(r2qColor(col));
	    pen.setWidthF(lwd);
	}
	painter.setPen(pen);
	if (fill != NA_INTEGER && !R_TRANSPARENT(fill)) {
	    painter.setBrush(QBrush(r2qColor(fill)));
	}

	painter.drawPolygon(points, n);
	free(points); // FIXME: use Calloc/Free?
    }
    else fprintf(stderr, "could not allocate %d 'QPointF's\n", n);
    current_device->update();
}


static void QT_Polyline(int n, double *x, double *y,
			R_GE_gcontext *gc,
			pDevDesc dev)
{
    QPainter painter(current_device->getPixmap());
    if (current_device->aaEdges()) painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRect(clip_rect);

    QPointF *points = (QPointF *) malloc(n * sizeof(QPointF));
    if (points != NULL) {
	for (int i = 0; i < n; i++) {
	    points[i] = QPointF(x[i], y[i]);
	}

	int col = gc->col;
	//double gamma = gc->gamma;
	double lwd = gc->lwd;
	int lty = gc->lty;

	//     R_GE_lineend lend;   /* Line end */
	//     R_GE_linejoin ljoin; /* line join */
	//     double lmitre;       /* line mitre */

	if (col != NA_INTEGER && !R_TRANSPARENT(col)) {
	    QPen pen;
	    pen.setStyle(lty2style(lty));
	    pen.setColor(r2qColor(col));
	    pen.setWidthF(lwd);
	    painter.setPen(pen);
	    painter.drawPolyline(points, n);
	}
	free(points);
    }
    else fprintf(stderr, "could not allocate %d 'QPointF's\n", n);
    current_device->update();
}


static void QT_Rect(double x0, double y0, double x1, double y1,
		    R_GE_gcontext *gc,
		    pDevDesc dev)
{
    QRectF rect(x0, y0, x1 - x0, y1 - y0);
    QPainter painter(current_device->getPixmap());
    painter.setClipRect(clip_rect);

    int col = gc->col;
    int fill = gc->fill;
    //double gamma = gc->gamma;
    double lwd = gc->lwd;
    int lty = gc->lty;

    QPen pen;
    if (col == NA_INTEGER || R_TRANSPARENT(col)) {
	// disable anti-aliasing (see image(volcano otherwise))
	painter.setRenderHint(QPainter::Antialiasing, false);
	pen.setStyle(Qt::NoPen);
    }
    else {
	if (current_device->aaEdges()) painter.setRenderHint(QPainter::Antialiasing);
	pen.setStyle(lty2style(lty));
	pen.setColor(r2qColor(col));
	pen.setWidthF(lwd);
    }
    painter.setPen(pen);
    if (fill != NA_INTEGER && !R_TRANSPARENT(fill)) {
	painter.setBrush(QBrush(r2qColor(fill)));
    }

    painter.drawRect(rect);
    current_device->update();
}

// FIXME: COMBINE COMMON PARTS OF NEXT TWO

// This no longer gets called, because everything is supposed to be
// sent as UTF8 (based on declared device capabilities)

static void QT_Text(double x, double y, char *str,
		    double rot, double hadj,
		    R_GE_gcontext *gc,
		    pDevDesc dev)
{

    Rprintf("QT_Text() called: This shouldn't happen\n");

    int col = gc->col;
    //double gamma = gc->gamma;
    double cex = gc->cex;
    double ps = gc->ps;
    double lineheight = gc->lineheight;
    int fontface = gc->fontface;
    char* fontfamily = gc->fontfamily; // [201] ??
    // char fontfamily[] = gc->fontfamily; // [201] ??

    QString qstr;
    if (fontface == 5) // symbol font, already properly encoded
	qstr = QString(str);
    else 
	qstr = QString::fromLocal8Bit(str);

    QPainter painter(current_device->getPixmap());
    if (current_device->aaText()) painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setClipRect(clip_rect);
    painter.setFont(r2qFont(fontfamily, fontface, ps, cex, lineheight,
			    current_device->defaultFamily()));
    painter.setPen(r2qColor(col));

    painter.translate(x, y);
    painter.rotate(-rot);


    QRectF brect = painter.boundingRect(current_device->rect(),
					Qt::AlignLeft | Qt::AlignBottom, qstr);
    painter.drawText(QPointF(-hadj * brect.width(), 0), qstr);
    current_device->update();
}



static void QT_TextUTF8(double x, double y, char *str,
			double rot, double hadj,
			R_GE_gcontext *gc,
			pDevDesc dev)
{
    int col = gc->col;
    //double gamma = gc->gamma;
    double cex = gc->cex;
    double ps = gc->ps;
    double lineheight = gc->lineheight;
    int fontface = gc->fontface;
    char* fontfamily = gc->fontfamily; // [201] ??
    // char fontfamily[] = gc->fontfamily; // [201] ??

    QString qstr = QString::fromUtf8(str);
    QPainter painter(current_device->getPixmap());
    if (current_device->aaText()) painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setClipRect(clip_rect);
    painter.setFont(r2qFont(fontfamily, fontface, ps, cex, lineheight,
			    current_device->defaultFamily()));
    painter.setPen(r2qColor(col));

    painter.translate(x, y);
    painter.rotate(-rot);

    QRectF brect = painter.boundingRect(current_device->rect(),
					Qt::AlignLeft | Qt::AlignBottom, qstr);
//     Rprintf("boundingRect: (left, bottom, width, height) = (%g,%g,%g,%g)\n",
// 	    brect.left(), brect.bottom(), brect.width(), brect.height());
    painter.drawText(QPointF(-hadj * brect.width(), 0), qstr);
    // feedback
//     painter.setPen(Qt::red);
//     painter.drawEllipse((int) x, (int) y, 5, 5);
//     Rprintf("text: '%s' at (%g,%g) with hadj: %g and rot: %g\n",
// 	    str, x, y, hadj, rot);
    current_device->update();
}




static void QT_NewPage(R_GE_gcontext *gc,
		       pDevDesc dev)
{

//     // FIXME: Work around R-2.7 update
//     saveGraphicsSnapshot(desc2GEDesc(dev)->savedSnapshot);
//     evaluateInR("qtutils:::saveQtPlot()");

    // printf("%g,%g,%g,%g\n", dev->left, dev->right, dev->top, dev->bottom);
    current_device->repaint();
    QApplication::processEvents();
    // FIXME: does this deal with par("bg")? 
    int fill = gc->fill;
    if (fill != NA_INTEGER && !R_TRANSPARENT(fill)) {
	// painter.fillRect(painter.viewport(), r2qColor(fill));
	current_device->getPixmap()->fill(r2qColor(fill));
    } else {
	current_device->getPixmap()->fill(Qt::transparent); // Qt::transparent
    }
    QPainter painter(current_device->getPixmap());
    // painter.eraseRect(current_device->rect());
    if (current_device->haveBackground())
	painter.drawImage(0, 0, current_device->background());
    // current_device->getPixmap()->fill(QColor(255, 255, 255, 0));
    // don't update unless you want a blank screen after plot.new
}


static void QT_Close(pDevDesc dev)
{
    // FIXME: how to distinguish between dev.off() and window manager
    // event (which invokes close() directly)
    current_device->setclosedByDevOff();
    current_device->close();
}


static void QT_Activate(pDevDesc dev)
{
    // GLOBAL  global_dev_ptr = current_device;
    current_device->setWindowTitle(QString("R/QT Device ") + 
				   QString::number(1 + current_device->getDeviceNumber()) + 
				   QString(" (ACTIVE)"));
}

static void QT_Clip(double left, 
		    double right, 
		    double bottom, 
		    double top,
		    pDevDesc dev)
{
//     Rprintf("Clip: (left, right, bottom, top) = (%g,%g,%g,%g)\n", 
// 	    left, right, bottom, top);
    dev->clipLeft   = left;
    dev->clipRight  = right;
    dev->clipBottom = bottom;
    dev->clipTop    = top;
}
static void QT_Deactivate(pDevDesc dev)
{
    current_device->setWindowTitle(QString("R/QT Device ") + 
				   QString::number(1 + current_device->getDeviceNumber()) + 
				   QString(" (INACTIVE)"));
}
static void QT_Mode(int mode, pDevDesc dev)
{
    Q_UNUSED(mode);
    Q_UNUSED(dev);
    /// too much flicker:

//     if (mode == 1) {
// 	current_device->setCursor(Qt::BusyCursor);
//     }
//     if (mode == 0) {
// 	current_device->setCursor(Qt::CrossCursor);
//     }

/// this may still be used at some point

//     if (mode == 0 && current_device->paintTimer()->isActive()) {
// 	// current_device->forceRepaint();
// 	current_device->paintTimer()->stop();
//     }
//     else if (mode == 1 && !current_device->paintTimer()->isActive()) {
// 	current_device->paintTimer()->start(1000); 
// 	// 1 second (FIXME: make customizable)
//     }
}

static Rboolean QT_Locator(double *x, double *y, pDevDesc dev)
{
    current_device->setContextMenuPolicy(Qt::PreventContextMenu);
    current_device->setClickPending();
    while (current_device->getClickPending()) {
#ifndef WIN32
	usleep(5);
#endif
 	QApplication::processEvents();
    }
    *x = current_device->lastClickX();
    *y = current_device->lastClickY();
    current_device->setContextMenuPolicy(Qt::ActionsContextMenu);
    if (current_device->leftButtonClicked()) return TRUE;
    else return FALSE;
}


// FIXME: doesn't work for some plotmath things at least (e.g. \sum)
static void QT_MetricInfo(int c,
			  R_GE_gcontext *gc,
			  double* ascent, double* descent,
			  double* width, pDevDesc dev)
{
    double cex = gc->cex;
    double ps = gc->ps;
    double lineheight = gc->lineheight;
    int fontface = gc->fontface;
    char* fontfamily = gc->fontfamily;
    bool Unicode = mbcslocale; // && (gc->fontface != 5);
    if (c < 0) { Unicode = TRUE; c = -c; }
//     if (gc->fontface == 5) {
// 	Rprintf("- c = %d [%0x], fontface = %d\n", c, c, gc->fontface);
//     }
//     if (!Unicode) {
// 	Rprintf("QT_MetricInfo: Not Unicode! This shouldn't happen\n");
// 	Rprintf("- c = %d [%0x], fontface = %d\n", c, c, gc->fontface);
//     }

    QPainter painter(current_device->getPixmap());
    painter.setFont(r2qFont(fontfamily, fontface, ps, cex, lineheight,
			    current_device->defaultFamily()));
    QFontMetrics fm = painter.fontMetrics();

    QChar uc = QChar(c);
    QRect bb = fm.boundingRect(uc); 
    // Note: bb.bottom() is probably negative. 
// WAS:
//     *ascent = (double) (bb.height() + bb.bottom());
//     *descent = (double) (-bb.bottom());
    *ascent = (double) (bb.height());
    *descent = (double) (0.0);
    *width = (double) fm.width(uc);

//     printf("(c, asc, desc, width) = (%d, %g, %g, %g)\n",
// 	   c, *ascent, *descent, *width);

    return;
}




static void QT_Size(double *left, double *right,
		    double *bottom, double *top,
		    pDevDesc dev)
{
    // FIXME FIXME FIXME : should I do this here?
    dev->left = 0;
    dev->right = current_device->width();
    dev->bottom = current_device->height();
    dev->top = 0;

    // Rprintf("Someone called QT_Size\n");
    *left = 0.0;
    *right = current_device->width();
    *bottom = current_device->height();
    *top = 0.0;
}


// This no longer gets called, because everything is supposed to be
// sent as UTF8 (based on declared device capabilities)

static double QT_StrWidth(char *str,
			  R_GE_gcontext *gc,
			  pDevDesc dev)
{

    Rprintf("QT_StrWidth() called: This shouldn't happen\n");

    double cex = gc->cex;
    double ps = gc->ps;
    double lineheight = gc->lineheight;
    int fontface = gc->fontface;
    char* fontfamily = gc->fontfamily;

    QString qstr;
    if (fontface == 5) // symbol font, already properly encoded
	qstr = QString(str);
    else 
	qstr = QString::fromLocal8Bit(str);

    QPainter painter(current_device->getPixmap());
    if (current_device->aaText()) painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setFont(r2qFont(fontfamily, fontface, ps, cex, lineheight,
			    current_device->defaultFamily()));
    QRectF brect = painter.boundingRect(current_device->rect(),
					Qt::AlignLeft | Qt::AlignBottom,
					qstr);
    return brect.width();
}


static double QT_StrWidthUTF8(char *str,
			      R_GE_gcontext *gc,
			      pDevDesc dev)
{
    double cex = gc->cex;
    double ps = gc->ps;
    double lineheight = gc->lineheight;
    int fontface = gc->fontface;
    char* fontfamily = gc->fontfamily;

    QString qstr = QString::fromUtf8(str);
    QPainter painter(current_device->getPixmap());
    if (current_device->aaText()) painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setFont(r2qFont(fontfamily, fontface, ps, cex, lineheight,
			    current_device->defaultFamily()));
    QRectF brect = painter.boundingRect(current_device->rect(),
					Qt::AlignLeft | Qt::AlignBottom,
					qstr);
    return brect.width();
}









extern "C" {

Rboolean
QTDeviceDriver(pDevDesc dev,
	       double width, double height,
	       double ps,
	       QtRDevice *qdev);



/* Device driver entry point */
Rboolean
QTDeviceDriver(pDevDesc dev,
	       double width, double height,
	       double ps,
	       QtRDevice *qdev)
{
    // store a pointer to the device object (in Qt), which can
    // subsequently be accessed through the current_device macro
    dev->deviceSpecific = (void *) qdev;
    // convert width and height to pixels and resize
    current_device->resize((int) (width * current_device->physicalDpiX()),
			   (int) (height * current_device->physicalDpiY()));
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
    dev->right  = dev->clipRight  = current_device->width();
    dev->bottom = dev->clipBottom = current_device->height();
    dev->top    = dev->clipTop    = 0;
    dev->cra[0] = 0.9 * ps;
    dev->cra[1] = 1.2 * ps;
    dev->xCharOffset = 0.4900;
    dev->yCharOffset = 0.3333;
    dev->yLineBias = 0.2;
    dev->ipr[0] = 1.0 / current_device->physicalDpiX();
    dev->ipr[1] = 1.0 / current_device->physicalDpiY();
    /*
     * Device capabilities
     */
    dev->canClip = TRUE;
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
    dev->strWidth =  (double (*)()) QT_StrWidth;
    dev->text =        (void (*)()) QT_Text;
    // dev->onexit =      (void (*)()) QT_OnExit; NULL is OK
    // dev->getEvent = SEXP (*getEvent)(SEXP, const char *);
    // dev->newFrameConfirm
    // dev->


    return TRUE;
}



typedef Rboolean
(*QtDeviceCreateFun)(pDevDesc,
		     double width, double height,
		     double ps,
		     QtRDevice *qdev);





static GEDevDesc*
createQtDevice(double width, double height,
	       double ps,
	       QtRDevice *qdev,
	       QtDeviceCreateFun init_fun)
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
	// gsetVar(install(".Device"), mkString("QT"), R_NilValue);
	// Rprintf("Started QT device\n");
	// dd = GEcreateDevDesc(dev);
        // dd->newDevStruct = 1;
	// addDevice((DevDesc*) dd);
	// GEinitDisplayList(dd);
	gdd = GEcreateDevDesc(dev);
	gdd->displayList = R_NilValue;
	gdd->savedSnapshot = R_NilValue;
	GEaddDevice2(gdd, "QT");
    } END_SUSPEND_INTERRUPTS;

    return(gdd);
}



static void
do_QT(double in_width,
      double in_height,
      double in_pointsize,
      QtRDevice *qdev)
{
    if (in_width <= 0 || in_height <= 0) {
	error("Invalid width or height in do_QT: (%g, %g)", in_width, in_height);
    }
    createQtDevice(in_width, in_height, in_pointsize, qdev, QTDeviceDriver);
}


// FIXME: should return value be indicative of success? (currently NULL).
// Note: the weird double* pointer stuff is because of .C() in ../R/qt.R

// int
// do_newQtDevice(double *width, double *height, double *pointsize)
// {
//     return startdev(*width, *height, *pointsize);
// }


SEXP
do_newQtDevice(SEXP width,
	       SEXP height,
	       SEXP pointsize,
	       SEXP family,
	       SEXP aaEdges, SEXP aaText,
	       SEXP bgimage)
{
    return wrapQWidget(startdev(asReal(width), 
				asReal(height), 
				asReal(pointsize), 
				CHAR(asChar(family)),
				(bool) asLogical(aaEdges),
				(bool) asLogical(aaText),
				CHAR(asChar(bgimage))));
}


SEXP 
saveCurrentPlotAsImage(SEXP filename)
{
    // this approach doesn't seem to work
//     int cur_dev_num = curDevice();
//     printf("current device: %d\n", cur_dev_num);
//     DevDesc* cur_dev = GetDevice(cur_dev_num);
//     Rprintf("current device again: %d\n", deviceNumber(cur_dev));
//     QtRDevice *qdev = (QtRDevice *) ((NewDevDesc* ) cur_dev)->deviceSpecific;

    QtRDevice *qdev = global_dev_ptr;
    const char *str = CHAR(asChar(filename));
    if (qdev->saveImageDirectly(str)) return ScalarLogical(1);
    else return ScalarLogical(0);
}


SEXP 
resizeCurrentDevice(SEXP width, SEXP height)
{
    QtRDevice *qdev = global_dev_ptr;
    qdev->setUpdatesEnabled(false);
    qdev->resize((int) (REAL(width)[0] * qdev->physicalDpiX()),
		 (int) (REAL(height)[0] * qdev->physicalDpiY()));
    qdev->setUpdatesEnabled(true);
    return R_NilValue;
}


} // end extern "C"





void QtRDevice::refreshDevice()
{
    // printf("dev number: %d\n", device_number);
    if (device_number > 0)
	GEplayDisplayList(GEgetDevice(device_number));
}


QtRDevice::QtRDevice(double width, double height,
		     double pointsize, 
		     const char *family,
		     bool edges_aa, bool text_aa,
		     const char *bgimage,
		     QWidget *parent, Qt::WFlags f)
    : QLabel(parent, f)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    setContextMenuPolicy(Qt::ActionsContextMenu); 

    do_QT(width, height, pointsize, this);
    default_family = QString(family);

    device_number = curDevice();
    click_pending = false;
    last_click_left_button = false;
    closed_by_dev_off = false;
    button_pressed = false;

    aa_edges = edges_aa;
    aa_text = text_aa;

    // read optional background image
    background_available = background_image.load(bgimage);

    // initialize pixmap.  Will be changed when device is resized etc.

#ifdef USE_PIXMAP
    pixmap = QPixmap(size());
    pixmap.fill(Qt::transparent); // Qt::transparent
    // pixmap.fill(QColor(255, 255, 255, 0));
#else
    pixmap = QImage(size(), QImage::Format_ARGB32_Premultiplied);
#endif

//     QPainter painter(&pixmap);
//     painter.eraseRect(size());

    // printf("device number at initialization: %d\n", device_number);
    setWindowTitle(QString("R/QT Device ") + 
		   QString::number(1 + device_number) + 
		   QString(" (ACTIVE)"));

    createActions();

    paint_timer = new QTimer(this);
    connect(paint_timer, SIGNAL(timeout()), 
	    this, SLOT(forceRepaint()));
    // doesn't seem to help
    // paint_timer->start(2500);
    repaint();
}


QtRDevice::~QtRDevice()
{
    delete paint_timer;
}



void QtRDevice::resizeEvent(QResizeEvent *e)
{
#ifdef USE_PIXMAP
    pixmap = QPixmap(size());
    pixmap.fill(Qt::transparent); // Qt::transparent
    // pixmap.fill(QColor(255, 255, 255, 0));
#else
    pixmap = QImage(size(), QImage::Format_ARGB32_Premultiplied);
#endif
    QWidget::resizeEvent(e);
    refreshDevice();
}


void QtRDevice::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    if (aaEdges()) painter.setRenderHint(QPainter::Antialiasing);
    if (aaText()) painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // painter.fillRect(rect(), QBrush(Qt::white));

#ifdef USE_PIXMAP
    painter.drawPixmap(rect(), pixmap, pixmap.rect());
#else
    painter.drawImage(rect(), pixmap, pixmap.rect());
#endif

    if (button_pressed && last_click_left_button) {
	painter.setBrush(QColor(127, 127, 144, 31));
	painter.setPen(QColor(127, 127, 144, 255));
	painter.drawRect(QRect(last_click, current_pos));
    }
    else if (crossHairAct->isChecked()) {
	painter.setPen(QColor(127, 127, 144, 255));
	painter.drawLine(current_pos.x(), 0, current_pos.x(), rect().height());
	painter.drawLine(0, current_pos.y(), rect().width(), current_pos.y());
    }
}

QTimer * QtRDevice::paintTimer()
{
    return paint_timer;
}

void QtRDevice::forceRepaint()
{
    // Rprintf("force repaint timeout\n");
    repaint();
    qApp->flush();
}


void QtRDevice::mousePressEvent(QMouseEvent *event)
{
    last_click = event->pos();
    button_pressed = true;
    last_click_left_button = (event->button() == Qt::LeftButton);
    QWidget::mousePressEvent(event);
}



void QtRDevice::mouseReleaseEvent(QMouseEvent *event)
{
    last_release = event->pos();
    if (button_pressed) {
	button_pressed = false;
	update();
    }
    if (click_pending) {
	click_pending = false;
    }
    QWidget::mouseReleaseEvent(event);
}


void QtRDevice::mouseMoveEvent(QMouseEvent *event)
{
    current_pos = event->pos();
    // if (button_pressed || crossHairAct->isChecked()) 
    update();
    QWidget::mouseMoveEvent(event);
}

void QtRDevice::keyPressEvent(QKeyEvent *event)
{
//     switch (event->key()) {
//     case Qt::Key_PageUp: 
// 	evaluateInR("qtutils::previousQtPlot()");
// 	break;
//     case Qt::Key_PageDown: 
// 	evaluateInR("qtutils::nextQtPlot()");
// 	break;
//     default:
// 	QWidget::keyPressEvent(event);
//     }
    QWidget::keyPressEvent(event);
}



void QtRDevice::closeEvent(QCloseEvent *event)
{
    if (!closedByDevOff()) {
	// printf("calling killDevice(%d)\n", device_number);
	killDevice(device_number);
    }
    // printf("closing device %d\n", device_number + 1);
    event->accept();
}

void QtRDevice::print()
{
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *printDialog = new QPrintDialog(&printer, this);

    if (printDialog->exec() == QDialog::Accepted) {
	QPainter painter(&printer);
	QRect rect = painter.viewport();
	QSize size = pixmap.size();
	size.scale(rect.size(), Qt::KeepAspectRatio);
	painter.setViewport(rect.x(), rect.y(), 
			    size.width(), size.height());
	painter.setWindow(pixmap.rect());
#ifdef USE_PIXMAP
	painter.drawPixmap(0, 0, pixmap);
#else
	painter.drawImage(0, 0, pixmap);
#endif
    }
}


void QtRDevice::about()
{
    QMessageBox::about(this, tr("About the QT Graphics Device"),
		       
		       tr("<p>The <b>QT</b> R Graphics device is an interactive screen "
			  "device like <code>X11()</code> on UNIX/Linux and "
			  "<code>windows()</code> on Microsoft Windows.  It"
			  "supports alpha transparency and unlike the old X11 (Xlib)"
			  "device, does not need a redraw at the R level on every expose event)."
			  "Fonts are handled transparently through QT, which allows "
			  "access to a wider selection of fonts and advanced typographical "
			  "features such as OpenType lookups with no extra effort.  "
			  "It is also cross-platform.</p> <p>Most features expected in a"
			  "screen device are available, including <code>locator</code>-like interaction."
			  "R line types are not fully supported, and plotmath behaviour "
			  "can be slightly inaccurate in some situations.  See <code>help(QT)</code> "
			  "for details on starting a QT device.  </p>"
			  "<p><em>Note:</em>  The QT device uses a bitmap image as its "
			  "backend storage.  Use <code>dev.copy</code> to save in vector "
			  "formats like PDF</p>"));
}


void QtRDevice::aboutInText()
{
    Rprintf((char*) ("The QT R Graphics device is an interactive screen.\nSee ?QT for details\n"));
}


void QtRDevice::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

bool QtRDevice::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
						    initialPath,
						    tr("%1 Files (*.%2);;All Files (*)")
						    .arg(QString(fileFormat.toUpper()))
						    .arg(QString(fileFormat)));
    if (fileName.isEmpty()) {
	return false;
    } else {
	return saveImage(fileName, fileFormat);
    }
}

bool QtRDevice::saveImage(const QString &fileName, const char *fileFormat)
{
    // should work in either case
// #ifdef USE_PIXMAP
// #else
// #endif
    return pixmap.save(fileName, fileFormat);
}


bool QtRDevice::saveImageDirectly(const char *file_name)
{
    const QString qfname = QString(file_name);
    return pixmap.save(qfname);
}



void QtRDevice::toggleCrossHairs()
{
    setMouseTracking(crossHairAct->isChecked());
    if (crossHairAct->isChecked()) setCursor(Qt::BlankCursor);
    else setCursor(Qt::CrossCursor);
}


void QtRDevice::createActions()
{

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    crossHairAct = new QAction(tr("Cross Hairs"), this);
    crossHairAct->setCheckable(true);
    connect(crossHairAct, SIGNAL(triggered()), 
	    this, SLOT(toggleCrossHairs()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    saveAsMenu = new QMenu(tr("&Save Bitmap As"), this);
    foreach (QByteArray format, QImageReader::supportedImageFormats()) {
	QString text = tr("%1...").arg(QString(format).toUpper());
	QAction *action = new QAction(text, this);
	action->setData(format);
	connect(action, SIGNAL(triggered()), this, SLOT(save()));
	saveAsMenu->addAction(action);
    }
    saveAsAction = saveAsMenu->menuAction();

    addAction(crossHairAct);
    addAction(printAct);
    addAction(saveAsAction);
    addAction(aboutAct);

}


