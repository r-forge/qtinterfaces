
#include <QWidget>
#include <QWebView>
#include <QUrl>
#include <QString>

#include <qtbase.h>

extern "C" {
SEXP qt_qwebView(SEXP url);
SEXP qt_qsetUrl(SEXP x, SEXP url);
SEXP qt_qurl(SEXP x);
}


SEXP
qt_qwebView(SEXP url)
{
    QWebView *view = new QWebView(0);
    view->setUrl(QString(CHAR(asChar(url))));
    return wrapQWidget(view);
}

SEXP
qt_qsetUrl(SEXP x, SEXP url)
{
    QWebView *w = unwrapQObject(x, QWebView);
    w->setUrl(QUrl(QString(CHAR(asChar(url)))));
    return R_NilValue;
}

SEXP
qt_qurl(SEXP x)
{
    QWebView *w = unwrapQObject(x, QWebView);
    return qstring2sexp(w->url().toString());
}

