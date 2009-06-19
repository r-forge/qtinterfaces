
if test -z "${QMAKE}" ; then
    QMAKE=qmake
fi

## REMEMBER: any changes below should also be applied to
## qtutils.pro.win

echo "

## NOTE: autogenerated by createProject.sh -- do not modify!!

TEMPLATE = lib

# Input

HEADERS += device.hpp helpers.hpp scenedevice.hpp

SOURCES += device.cpp init.c imports.cpp helpers.cpp
SOURCES += scenedevice.cpp

INCLUDEPATH += ${R_INCLUDE_DIR}
INCLUDEPATH += ${CLINK_CPPFLAGS/-I/}

macx{
LIBS += -framework R
}
unix{
LIBS += -L${R_HOME}/lib -lR
}
win32{
LIBS += -L${R_HOME}/bin -lR
}


" > qtdevice.pro

if test `uname` == "Darwin" ; then
    ${QMAKE} -spec macx-g++ -o Makefile.qtdevice
else 
    ${QMAKE} -o Makefile.qtdevice
fi

${QMAKE} -o Makefile.qtdevice

