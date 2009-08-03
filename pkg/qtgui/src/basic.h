#ifndef QTGUI_BASIC_H
#define QTGUI_BASIC_H

SEXP qt_qwidget();
SEXP qt_qpushButton(SEXP);
SEXP qt_qsetTextButton(SEXP,SEXP);
SEXP qt_qtextButton(SEXP);
SEXP qt_qlabel(SEXP);
SEXP qt_qsetTextLabel(SEXP,SEXP);
SEXP qt_qtextLabel(SEXP);
SEXP qt_qlineEdit(SEXP);
SEXP qt_qsetTextLineEdit(SEXP,SEXP);
SEXP qt_qtextLineEdit(SEXP);
SEXP qt_qcheckBox(SEXP);
SEXP qt_qisCheckedButton(SEXP);
SEXP qt_qsetCheckedButton(SEXP,SEXP);

#endif

