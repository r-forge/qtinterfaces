
#ifndef QTGUI_WRAPPERS_H
#define QTGUI_WRAPPERS_H


SEXP qt_qshow(SEXP x);
SEXP qt_qupdate_QWidget(SEXP x);
SEXP qt_qclose(SEXP x);
SEXP qt_qraise(SEXP x);
SEXP qt_qlower(SEXP x);
SEXP qt_qparent(SEXP x);
SEXP qt_qsetParent(SEXP x, SEXP parent);
SEXP qt_qresize(SEXP x, SEXP w, SEXP h);
SEXP qt_qheight(SEXP x);
SEXP qt_qwidth(SEXP x);
SEXP qt_qsetMinimumSize(SEXP x, SEXP w, SEXP h);
SEXP qt_setExpanding(SEXP x, SEXP vertical, SEXP horizontal);
SEXP qt_qsetContentsMargins(SEXP x, SEXP left, SEXP top, SEXP right, SEXP bottom);
SEXP qt_qisEnabled(SEXP x);
SEXP qt_qsetEnabled(SEXP x, SEXP flag);
SEXP qt_qwindowTitle(SEXP x);
SEXP qt_qsetWindowTitle(SEXP x, SEXP title);
SEXP qt_qrender(SEXP x, SEXP file);
SEXP qt_qrenderGraphicsView(SEXP x);
SEXP qt_qrenderToPixmap(SEXP x, SEXP file);
SEXP qt_qrenderToSVG(SEXP x, SEXP file);

#endif

