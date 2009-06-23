
#ifndef QTDEVICE_HELPERS_H
#define QTDEVICE_HELPERS_H


#include <QFont>
#include <QColor>
#include <QString>

#define R_USE_PROTOTYPES 0


// #include <R.h>
// #include <Rinternals.h>

// #include <R_ext/GraphicsEngine.h>
// #include <R_ext/GraphicsDevice.h>
// #include <R_ext/Error.h>

// #ifndef WIN32
// #include <unistd.h>
// #endif

#define r2qColor(col) QColor(R_RED(col), R_GREEN(col), R_BLUE(col), R_ALPHA(col))

QFont
r2qFont(const char *fontfamily, int fontface,
	double ps, double cex, 
	double lineheight, // ?? for multiple lines?
	QString dff);

Qt::PenStyle 
lty2style(int lty);


#endif
