
#include <QWidget>
#include <QAbstractButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

#include <qtbase.h>

extern "C" {
#include "basic.h"
}

// Next in line to implement:

// QComboBox	Combined button and popup list



// QCheckBox	Checkbox with a text label
// QComboBox	Combined button and popup list
// QDial	Rounded range control (like a speedometer or potentiometer)
// QDoubleSpinBox	Spin box widget that takes doubles
// QFontComboBox	Combobox that lets the user select a font family
// QLCDNumber	Displays a number with LCD-like digits
// QLabel	Text or image display
// QLineEdit	One-line text editor
// QMenu	Menu widget for use in menu bars, context menus, and other popup menus
// QProgressBar	Horizontal or vertical progress bar
// QPushButton	Command button
// QRadioButton	Radio button with a text label
// QScrollArea	Scrolling view onto another widget
// QScrollBar	Vertical or horizontal scroll bar
// QSlider	Vertical or horizontal slider
// QSpinBox	Spin box widget
// QTabBar	Tab bar, e.g. for use in tabbed dialogs
// QTimeEdit	Widget for editing times based on the QDateTimeEdit widget
// QToolBox	Column of tabbed widget items
// QToolButton	Quick-access button to commands or options, usually used inside a QToolBar

SEXP 
qt_qwidget() {
    return wrapQWidget(new QWidget());
}


SEXP 
qt_qpushButton(SEXP s) {
    return wrapQWidget(new QPushButton(sexp2qstring(s)));
}

SEXP 
qt_qsetText_QAbstractButton(SEXP x, SEXP s) {
    unwrapQObject(x, QAbstractButton)->setText(sexp2qstring(s));
    return R_NilValue;
}

SEXP 
qt_qtext_QAbstractButton(SEXP x) {
    return qstring2sexp(unwrapQObject(x, QAbstractButton)->text());
}

SEXP 
qt_qlabel(SEXP label) {
    return wrapQWidget(new QLabel(sexp2qstring(label), 0));
}

SEXP 
qt_qsetText_QLabel(SEXP x, SEXP s) {
    unwrapQObject(x, QLabel)->setText(sexp2qstring(s));
    return R_NilValue;
}

SEXP 
qt_qtext_QLabel(SEXP x) {
    return qstring2sexp(unwrapQObject(x, QLabel)->text());
}


SEXP 
qt_qlineEdit(SEXP s) {
    QLineEdit *x = new QLineEdit();
    if (s != R_NilValue) x->setText(sexp2qstring(s));
    return wrapQWidget(x);
}

SEXP 
qt_qsetText_QLineEdit(SEXP x, SEXP s) {
    unwrapQObject(x, QLineEdit)->setText(sexp2qstring(s));
    return R_NilValue;
}

SEXP 
qt_qtext_QLineEdit(SEXP x) {
    return qstring2sexp(unwrapQObject(x, QLineEdit)->text());
}

SEXP 
qt_qcheckBox(SEXP label) {
    if (label == R_NilValue) return wrapQWidget(new QCheckBox(0));
    else return wrapQWidget(new QCheckBox(sexp2qstring(label), 0));
}


