#include <QFontMetrics>
#include <qtbase.h>

extern "C" {
  #include "fonts.h"
}

SEXP qt_qtextExtents_QFont(SEXP rfont, SEXP rtext) {
  QRectF boundingRect =
    QFontMetrics(asQFont(rfont)).boundingRect(sexp2qstring(rtext));
  return asRRectF(boundingRect);
}
