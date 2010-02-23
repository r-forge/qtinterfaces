#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsWidget>

#include "PlotView.hpp"

class Painter;
class ScenePainter;
class QGraphicsGridLayout;

namespace Qanviz {
  class Layer : public QGraphicsWidget {
    Q_OBJECT
    
    private:
    
    QRectF _limits;
    QGraphicsScene *indexScene;
    ScenePainter *scenePainter;
    
    // QGraphicsWidget assumes a (0, 0, width, height) bounding
    // rectangle, where width and height are from the geometry.
    // We want to draw into data space, so we allow the
    // user to specify the data limits, and then synchronize the item
    // transform so that everything ends up in the geometry specified
    // by the layout.

    void updatePlotTransform();
    
    QVector<int> itemIndices(QList<QGraphicsItem *> items);
    
    public:
    
    Layer(QGraphicsItem *parent);
    virtual ~Layer();
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    virtual void paintPlot(Painter *painter, QRectF exposed) = 0;

    void setGeometry(const QRectF &rect) {
      QGraphicsWidget::setGeometry(rect);    
      // update transform to point current limits to new geometry
      updatePlotTransform();
    }

    QRectF boundingRect() const {
      if (!_limits.isNull()) {
        return _limits;
      } else return QGraphicsWidget::boundingRect();
    }

    QTransform deviceTransform(QGraphicsView *view = NULL) const;
    
    QPainterPath shape() const {
      if (!_limits.isNull()) {
        QPainterPath path;
        path.addRect(_limits);
        return path;
      } else return QGraphicsWidget::shape();
    }
    
    QGraphicsGridLayout *gridLayout() const;

    void addLayer(Layer *layer, int row, int col, int rowSpan, int colSpan);

    virtual void setLimits(QRectF limits) {
      if (limits != _limits) {
        prepareGeometryChange();
        _limits = limits;
        // update transform to point new limits to current geometry
        updatePlotTransform();
      }
    }
    QRectF limits() const {
      return _limits;
    }

    void setItemIndexMethod(QGraphicsScene::ItemIndexMethod method) {
      indexScene->setItemIndexMethod(method);
    }
    QGraphicsScene::ItemIndexMethod itemIndexMethod() {
      return indexScene->itemIndexMethod();
    }
    
    void ensureIndex();
    void invalidateIndex();
    
    QVector<int> primitives(const QPointF & pos)
    {
      ensureIndex();
      return itemIndices(indexScene->items(pos));
    }
    
    QVector<int> primitives(const QRectF & rectangle,
                            Qt::ItemSelectionMode mode =
                            Qt::IntersectsItemShape)
    {
      ensureIndex();
      return itemIndices(indexScene->items(rectangle, mode));
    }

    
    QVector<int> primitives(const QPolygonF & polygon,
                            Qt::ItemSelectionMode mode =
                            Qt::IntersectsItemShape)
    {
      ensureIndex();
      return itemIndices(indexScene->items(polygon, mode));
    }
    
    QVector<int> primitives(const QPainterPath & path,
                            Qt::ItemSelectionMode mode =
                            Qt::IntersectsItemShape)
    {
      ensureIndex();
      return itemIndices(indexScene->items(path, mode));
    }
    
    QVector<int>
    collidingItems(const QGraphicsItem *item,
                   Qt::ItemSelectionMode mode = Qt::IntersectsItemShape)
    {
      ensureIndex();
      return itemIndices(indexScene->collidingItems(item, mode));
    }

    static QGraphicsView *viewForEvent(QGraphicsSceneEvent *event);

  };
}

#endif
