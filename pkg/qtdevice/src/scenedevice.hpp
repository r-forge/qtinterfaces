
#ifndef QTDEVICE_SCENE_H
#define QTDEVICE_SCENE_H

#include <QWidget>
#include <QGraphicsScene>

#include "helpers.hpp"


class RSceneDevice : public QGraphicsScene
{
    Q_OBJECT

 private:

    bool debug;
    int device_number;
    QString default_family;

 public:

    RSceneDevice(double width, double height, 
		 double pointsize, const char *family);
    // ~RSceneDevice(); 

    QString defaultFamily() { return default_family; }
    void setDeviceNumber(int n) { device_number = n; }
    int getDeviceNumber() { return device_number; }

    void Circle(double x, double y, double r, 
		R_GE_gcontext *gc);
    void Line(double x1, double y1, double x2, double y2,
	      R_GE_gcontext *gc);
    void Polygon(int n, double *x, double *y,
		 R_GE_gcontext *gc);
    void Polyline(int n, double *x, double *y,
		  R_GE_gcontext *gc);
    void Rect(double x0, double y0, double x1, double y1,
	      R_GE_gcontext *gc);
    void TextUTF8(double x, double y, char *str,
		  double rot, double hadj,
		  R_GE_gcontext *gc);
    void NewPage(R_GE_gcontext *gc);
    void MetricInfo(int c, R_GE_gcontext *gc,
		    double* ascent, double* descent,
		    double* width);
    double StrWidthUTF8(char *str, R_GE_gcontext *gc);



//  public slots:


//  protected:
//     void paintEvent(QPaintEvent *event); // should simply copy pixmap to label
//     void resizeEvent(QResizeEvent *e);
//     void mousePressEvent(QMouseEvent *event);
//     void mouseReleaseEvent(QMouseEvent *event);
//     void mouseMoveEvent(QMouseEvent *event);
//     void keyPressEvent(QKeyEvent *event);
//     void closeEvent(QCloseEvent *event);

};




extern "C" {

Rboolean
RSceneDeviceDriver(pDevDesc dev,
		   double width, double height,
		   double ps,
		   RSceneDevice *qdev);

SEXP
qt_qsceneDevice(SEXP width,
		SEXP height,
		SEXP pointsize,
		SEXP family);

SEXP
qt_qsceneView(SEXP x); 



}


#endif
