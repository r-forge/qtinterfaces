
#ifndef QTGUI_BASIC_H
#define QTGUI_BASIC_H

SEXP qt_qwidget();
SEXP qt_qpushButton(SEXP s);
SEXP qt_qsetTextButton(SEXP x, SEXP s);
SEXP qt_qtextButton(SEXP x);
SEXP qt_qlabel(SEXP label);
SEXP qt_qsetTextLabel(SEXP x, SEXP s);
SEXP qt_qtextLabel(SEXP x);
SEXP qt_qlineEdit(SEXP s);
SEXP qt_qsetTextLineEdit(SEXP x, SEXP s);
SEXP qt_qtextLineEdit(SEXP x);
SEXP qt_qcheckBox(SEXP label);
SEXP qt_qisCheckedButton(SEXP x);
SEXP qt_qsetCheckedButton(SEXP x, SEXP status);


#endif

