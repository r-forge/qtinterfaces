

#include "handler.hpp"

#include <qtbase.h>

#include <R.h>
#include <Rinternals.h>
#include <R_ext/GraphicsEngine.h>
#include <R_ext/GraphicsDevice.h>



RSignalHandler::RSignalHandler(QObject *src,
			       SEXP data,
			       SEXP handler, 
			       SEXP which)
    : QObject(src) // 0 
    // will QObject(src)? Will that keep the handler around as long as
    // its parent lives?
{
    user_data = data;
    handler_function = handler;
    R_PreserveObject(user_data);
    R_PreserveObject(handler_function);
    QString w = sexp2qstring(which);
    bool status;
    if (w == "clicked_bool") // QAbstractButton
	status = connect(src, SIGNAL(clicked(bool)),
			 this, SLOT(handle(bool)));
    if (w == "triggered_bool") // QAction
	status = connect(src, SIGNAL(triggered(bool)),
			 this, SLOT(handle(bool)));
    else if (w == "destroyed_qobject") // QWidget?
	status = connect(src, SIGNAL(destroyed(QObject*)),
			 this, SLOT(handle(QObject*)));
//     else if (w == "cellPressed_int_int") // QTableWidget
// 	status = connect(src, SIGNAL(cellPressed(int, int)),
// 			 this, SLOT(handle(int, int)));
    else if (w == "cellClicked_int_int") // QTableWidget
	status = connect(src, SIGNAL(cellClicked(int, int)),
			 this, SLOT(handle(int, int)));
    else if (w == "cellActivated_int_int") // QTableWidget
	status = connect(src, SIGNAL(cellActivated(int, int)),
			 this, SLOT(handle(int, int)));
    else if (w == "cellDoubleClicked_int_int") // QTableWidget
	status = connect(src, SIGNAL(cellDoubleClicked(int, int)),
			 this, SLOT(handle(int, int)));
    else if (w == "itemClicked_qlistwidgetitem") // QListWidget
	status = connect(src, SIGNAL(itemClicked(QListWidgetItem*)),
			 this, SLOT(handle(QListWidgetItem*)));
    else if (w == "itemActivated_qlistwidgetitem") // QListWidget
	status = connect(src, SIGNAL(itemActivated(QListWidgetItem*)),
			 this, SLOT(handle(QListWidgetItem*)));
    else status = false;
    if (!status) warning("Connection unsuccessful");
}

RSignalHandler::~RSignalHandler()
{
    R_ReleaseObject(user_data);
    R_ReleaseObject(handler_function);
}

void RSignalHandler::evaluateHandler(SEXP f)
{
    SEXP R_fcall;
    if (R_fcall == R_NilValue) return;
    if (!isFunction(f)) error("handler must be a function");
    PROTECT(R_fcall = lang2(f, R_NilValue));
    SETCADR(R_fcall, user_data);
    BEGIN_SUSPEND_INTERRUPTS {
	// eval(R_fcall, R_GlobalEnv);
	R_tryEval(R_fcall, R_GlobalEnv, NULL);
    } END_SUSPEND_INTERRUPTS;
    UNPROTECT(1);
    return;
}

void RSignalHandler::handle(bool checked)
{
    evaluateHandler(handler_function);
}

void RSignalHandler::handle(QObject *obj)
{
    evaluateHandler(handler_function);
}

void RSignalHandler::handle(int row, int column)
{
    evaluateHandler(handler_function);
}

void RSignalHandler::handle(QListWidgetItem*)
{
    evaluateHandler(handler_function);
}



