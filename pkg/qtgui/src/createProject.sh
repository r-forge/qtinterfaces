
if test -z "${QMAKE}" ; then
	if test -f /usr/bin/qmake-qt4; then
		QMAKE=qmake-qt4
	else
		QMAKE=qmake
	fi
fi

## REMEMBER: any changes below should also be applied to
## qtgui.pro.win

echo "

## NOTE: autogenerated by createProject.sh -- do not modify!!

TEMPLATE = lib

QT += svg

# Input

HEADERS += basic.h containers.h layout.h splitter.h qwidgetWrappers.h graphicsView.h
HEADERS += fonts.h

SOURCES += init.c imports.cpp basic.cpp containers.cpp layout.cpp splitter.cpp
SOURCES += qwidgetWrappers.cpp attributes.cpp actions.cpp
SOURCES += graphicsView.cpp fonts.cpp

INCLUDEPATH += ${R_INCLUDE_DIR}
INCLUDEPATH += ${CLINK_CPPFLAGS/-I/}


#CFLAGS += -g
#CPPFLAGS += ${CLINK_CPPFLAGS}

## LIBS += -L${R_HOME/lib}


macx{
LIBS += -framework R
}
unix{
LIBS += -L${R_HOME}/lib -lR
}
win32{
LIBS += -L${R_HOME}/bin -lR
}

" > qtgui.pro

if test `uname` == "Darwin" ; then
    ${QMAKE} -spec macx-g++ -o Makefile.qtgui
else 
    ${QMAKE} -o Makefile.qtgui
fi

## if macx; do 
##     ${QMAKE} -spec macx-g++ -o Makefile.qtgui
## else



