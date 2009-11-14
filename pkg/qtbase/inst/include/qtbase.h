#ifndef QTBASE_H
#define QTBASE_H

// FIXME: we should not have duplicate header files inside and outside
// the package. Could use symbolic links.

#include <QObject>
#include <QWidget>
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsLayoutItem>

#include <Rinternals.h>

#include <qtdefs.h>

QObject *unwrapQObjectReferee(SEXP x);
QGraphicsItem *unwrapQGraphicsItemReferee(SEXP x);
QGraphicsLayoutItem *unwrapQGraphicsLayoutItemReferee(SEXP x);

#define checkPointer(x, type) ({                                       \
      if (TYPEOF(x) != EXTPTRSXP)                                      \
        error("checkPointer: not an externalptr");                     \
      if (!inherits(x, #type))                                         \
        error("checkPointer: expected object of class '" #type "'");   \
    })

#define unwrapPointerSep(x, rtype, ctype) ({                            \
      checkPointer(x, rtype);                                           \
      reinterpret_cast<ctype *>(R_ExternalPtrAddr(x));                  \
    })

#define unwrapPointer(x, type) unwrapPointerSep(x, type, type)

#define unwrapSmoke(x, type) reinterpret_cast<type *>(_unwrapSmoke(x, #type))

#define unwrapReference(x, type) ({                                     \
      type *ans =                                                       \
        qobject_cast<type *>(unwrapPointer(x, type));                   \
      if (!ans || !ans->isValid())                                      \
        error("unwrapReference: Coercion to '" #type "' failed");       \
      ans;                                                              \
    })

#define unwrapQObject(x, type) ({                                       \
      type *ans = qobject_cast<type *>(unwrapQObjectReferee(x));	\
      if (!ans) error("unwrapQObject: Coercion to " #type " failed");	\
      ans;								\
    })

#define unwrapQGraphicsItem(x, type) ({                                 \
      type *ans = qgraphicsitem_cast<type *>(unwrapQGraphicsItemReferee(x)); \
      if (!ans) error("unwrapQGraphicsItem: Coercion to " #type " failed"); \
      ans;                                                              \
    })

#define unwrapQGraphicsLayoutItem(x, type) \
  reinterpret_cast<type *>(unwrapQGraphicsLayoutItemReferee(x))

#define unwrapQGraphicsWidget(x) unwrapQGraphicsItem(x, QGraphicsWidget)
#define unwrapQWidget(x) unwrapQObject(x, QWidget)

QT_BEGIN_DECLS

void *_unwrapSmoke(SEXP x, const char *type);

SEXP wrapQObject(QObject *object);
SEXP wrapQWidget(QWidget *widget);
SEXP wrapPointer(void *ptr, QList<QString> classNames = QList<QString>(),
                 R_CFinalizer_t finalizer = NULL);
SEXP wrapQGraphicsItem(QGraphicsItem *item,
                       QList<QString> classNames = QList<QString>());
SEXP wrapQGraphicsWidget(QGraphicsWidget *widget);
SEXP wrapQGraphicsLayoutItem(QGraphicsLayoutItem *item, QList<QString> classes);

// Conversion routines
// R -> C/Qt
const char ** asStringArray(SEXP s_strs);
QString sexp2qstring(SEXP s);

QRectF asQRectF(SEXP r);
QPointF asQPointF(SEXP p);
QSizeF asQSizeF(SEXP s);
QMatrix asQMatrix(SEXP m);

QColor *asQColors(SEXP c);
QColor asQColor(SEXP c);
QFont asQFont(SEXP f);

// C -> R
SEXP asRStringArray(const char * const * strs);
SEXP qstring2sexp(QString s);

SEXP asRRectF(QRectF rect);
SEXP asRMatrix(QMatrix matrix, bool inverted);
SEXP asRPointF(QPointF point);
SEXP asRSizeF(QSizeF size);

SEXP asRColor(QColor color);
SEXP asRFont(QFont font);

// Reference counting
void addQObjectReference(QObject *referee, QObject *referer);
void addQWidgetReference(QWidget *referee, QObject *referer);
void addQGraphicsItemReference(QGraphicsItem *referee, QObject *referer);
void addQGraphicsLayoutItemReference(QGraphicsLayoutItem *referee,
                                     QObject *referer);

QT_END_DECLS


#endif
