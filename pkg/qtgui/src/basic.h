#ifndef QTGUI_BASIC_H
#define QTGUI_BASIC_H

SEXP qt_qwidget();
SEXP qt_qpushButton(SEXP);
SEXP qt_qsetText_QAbstractButton(SEXP,SEXP);
SEXP qt_qtext_QAbstractButton(SEXP);
SEXP qt_qlabel(SEXP);
SEXP qt_qsetText_QLabel(SEXP,SEXP);
SEXP qt_qtext_QLabel(SEXP);
SEXP qt_qlineEdit(SEXP);
SEXP qt_qsetText_QLineEdit(SEXP,SEXP);
SEXP qt_qtext_QLineEdit(SEXP);
SEXP qt_qcheckBox(SEXP);

#endif

