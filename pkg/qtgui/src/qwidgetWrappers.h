#ifndef QTGUI_QWIDGETWRAPPERS_H
#define QTGUI_QWIDGETWRAPPERS_H

SEXP qt_qshow(SEXP);
SEXP qt_qupdate_QWidget(SEXP);
SEXP qt_qclose(SEXP);
SEXP qt_qraise(SEXP);
SEXP qt_qlower(SEXP);
SEXP qt_qparent(SEXP);
SEXP qt_qsetParent(SEXP,SEXP);
SEXP qt_qresize(SEXP,SEXP,SEXP);
SEXP qt_qheight(SEXP);
SEXP qt_qwidth(SEXP);
SEXP qt_qsetMinimumSize(SEXP,SEXP,SEXP);
SEXP qt_setExpanding(SEXP,SEXP,SEXP);
SEXP qt_qsetContentsMargins(SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qisEnabled(SEXP);
SEXP qt_qsetEnabled(SEXP,SEXP);
SEXP qt_qwindowTitle(SEXP);
SEXP qt_qsetWindowTitle(SEXP,SEXP);
SEXP qt_qrender(SEXP,SEXP);
SEXP qt_qrenderGraphicsView(SEXP);
SEXP qt_qrenderToPixmap(SEXP,SEXP);
SEXP qt_qrenderToSVG(SEXP,SEXP);

#endif

