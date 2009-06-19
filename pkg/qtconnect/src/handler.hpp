
#ifndef QT_HANDLER_H
#define QT_HANDLER_H

#include <QObject>
#include <QListWidgetItem>

#include <R.h>
#include <Rinternals.h>

class RSignalHandler : public QObject
{
    Q_OBJECT

 private:

    SEXP user_data;
    SEXP handler_function;

 public:

    RSignalHandler(QObject *src,
		   SEXP data,
		   SEXP handler,
		   SEXP which);
    ~RSignalHandler();
    void evaluateHandler(SEXP f);

 public slots:

    void handle(bool checked);
    void handle(QObject *obj);
    void handle(int row, int columns);
    void handle(QListWidgetItem *item);

};


#endif

