#ifndef PLOTVIEW_H
#define PLOTVIEW_H 1

#include <QGraphicsView>

#include "Layer.hpp"


/* Specialized GraphicsView for plotting.
   - automatic rescaling
   - 'overlay' as a scene drawn in viewport coordinates in the foreground.
*/
namespace QViz {

  class PlotView;

  /* A per-view collection of items to be rendered in device
     coordinates, in the foreground. Does not accept events. */
  class OverlayScene : public QGraphicsScene {
    Q_OBJECT
    
  private:
    QGraphicsView *_view;
  
  public:
    QGraphicsView * view() { return _view; }

  protected:
    OverlayScene(QGraphicsView *view) : QGraphicsScene(view), _view(view) { }
    void drawOverlay(QPainter *painter, QRectF exposed);

    friend class PlotView;
  };

  class PlotView : public QGraphicsView {
    Q_OBJECT

  public:

    enum RescaleMode {
      None,
      WidgetGeometry, /* activates layout, single view only */
      ViewTransform /* for rescaling with multiple views - useful? */
    };
    
    PlotView(QGraphicsScene *scene, QWidget *parent = 0,
             RescaleMode rescaleMode = WidgetGeometry)
      : QGraphicsView(scene, parent), _overlay(new OverlayScene(this))
    {
      setRescaleMode(rescaleMode);
      connect(_overlay, SIGNAL(changed(QList<QRectF>)), this,
              SLOT(update()));
    }

    //virtual ~PlotView();
    
    void setRescaleMode(RescaleMode rescaleMode) {
      if (rescaleMode > None) {
        // NOTE: seems only to have effect after widget is visible
        Qt::ScrollBarPolicy policy = Qt::ScrollBarAlwaysOn;
        setHorizontalScrollBarPolicy(policy);
        setVerticalScrollBarPolicy(policy);
        if (rescaleMode == ViewTransform)
          setResizeAnchor(QGraphicsView::AnchorViewCenter);
        else setResizeAnchor(QGraphicsView::NoAnchor);
      }
      _rescaleMode = rescaleMode;
    }
    bool rescaleMode() const {
      return _rescaleMode;
    }

    OverlayScene *overlay() {
      return _overlay;
    }
    
    bool isPainting() {
      return _isPainting;
    }

    
  protected:

    void resizeEvent (QResizeEvent * event);
    void paintEvent (QPaintEvent * event);
    
    void drawForeground(QPainter *painter, const QRectF &rect);
    //void drawBackground(QPainter *painter, const QRectF &rect);
    
  private:
        
    RescaleMode _rescaleMode;
    QSizeF oldSize;
    OverlayScene *_overlay;
    bool _isPainting;

    void updateGeometry(QGraphicsScene *scene);
    void drawMetaScene(OverlayScene *scene, QPainter *painter, QRectF rect);
  };
}

#endif
