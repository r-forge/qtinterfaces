
#ifndef QTRDEVICE_H
#define QTRDEVICE_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QMenu>

#include <R.h>
#include <Rinternals.h>
#define R_USE_PROTOTYPES 0
#include <R_ext/GraphicsEngine.h>
#include <R_ext/GraphicsDevice.h>



#define USE_PIXMAP


class QtRDevice : public QLabel
{
    Q_OBJECT

 private:

#ifdef USE_PIXMAP
    QPixmap pixmap; // holds all drawing data
#else
    QImage pixmap; // holds all drawing data
#endif

    int device_number;
    QString default_family;
    bool click_pending;
    bool last_click_left_button;
    QPoint last_click;
    QPoint current_pos;
    QPoint last_release;
    bool button_pressed;
    bool closed_by_dev_off;

    void createActions();
    QAction *aboutAct;
    QAction *crossHairAct;
    QAction *printAct;
    QMenu *saveAsMenu;
    QAction *saveAsAction;

    QTimer *paint_timer;
    bool saveFile(const QByteArray &fileFormat);
    bool saveImage(const QString &fileName, const char *fileFormat = 0);

    QImage background_image;
    bool background_available;

    // anti-aliasing flags
    bool aa_edges;
    bool aa_text;
    

 public:

    QtRDevice(double width, double height, 
	      double pointsize, const char *family, 
	      bool edges_aa, bool text_aa,
	      const char *bgimage, 
	      QWidget *parent = 0, Qt::WFlags f = 0);
    ~QtRDevice(); 

    void refreshDevice(); // get R to redraw 
    QTimer *paintTimer();
    QString defaultFamily() { return default_family; }
    void setDeviceNumber(int n) { device_number = n; }
    int getDeviceNumber() { return device_number; }
    void setClickPending() { click_pending = true; }
    bool getClickPending() { return click_pending; }
    double lastClickX() { return (double) last_click.x(); }
    double lastClickY() { return (double) last_click.y(); }
    bool leftButtonClicked() { return last_click_left_button; }
    void setclosedByDevOff() { closed_by_dev_off = true; }
    bool closedByDevOff() { return closed_by_dev_off; }
    bool haveBackground() { return background_available; }
    QImage background() { return background_image; }
    bool saveImageDirectly(const char *file_name);

    bool aaText() { return aa_text; }
    bool aaEdges() { return aa_edges; }


#ifdef USE_PIXMAP
    QPixmap *getPixmap() { return &pixmap; }
#else
    QImage *getPixmap() { return &pixmap; }
#endif

 public slots:

    void forceRepaint();
    void print();
    void about();
    void aboutInText();
    void save();
    void toggleCrossHairs();

 protected:
    void paintEvent(QPaintEvent *event); // should simply copy pixmap to label
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

};


#endif
