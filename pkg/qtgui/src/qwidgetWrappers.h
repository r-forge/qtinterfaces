#ifndef QTGUI_QWIDGETWRAPPERS_H
#define QTGUI_QWIDGETWRAPPERS_H

SEXP qt_qshow_QWidget(SEXP);
SEXP qt_qupdate_QWidget(SEXP);
SEXP qt_qclose_QWidget(SEXP);
SEXP qt_qraise_QWidget(SEXP);
SEXP qt_qlower_QWidget(SEXP);
SEXP qt_qparent_QWidget(SEXP);
SEXP qt_qsetParent_QWidget(SEXP,SEXP);
SEXP qt_qresize_QWidget(SEXP,SEXP,SEXP);
SEXP qt_qheight_QWidget(SEXP);
SEXP qt_qwidth_QWidget(SEXP);
SEXP qt_qsetMinimumSize_QWidget(SEXP,SEXP,SEXP);
SEXP qt_qsetExpanding_QWidget(SEXP,SEXP,SEXP);
SEXP qt_qsetContentsMargins_QWidget(SEXP,SEXP,SEXP,SEXP,SEXP);
SEXP qt_qisEnabled_QWidget(SEXP);
SEXP qt_qsetEnabled_QWidget(SEXP,SEXP);
SEXP qt_qwindowTitle_QWidget(SEXP);
SEXP qt_qsetWindowTitle_QWidget(SEXP,SEXP);
SEXP qt_qrender_QWidget(SEXP,SEXP);
SEXP qt_qrender_QGraphicsView(SEXP);
SEXP qt_qrenderToPixmap(SEXP,SEXP);
SEXP qt_qrenderToSVG(SEXP,SEXP);

#endif

