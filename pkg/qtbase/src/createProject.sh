
if test -z "${QMAKE}" ; then
    QMAKE=qmake
fi

## REMEMBER: any changes below should also be applied to
## qtbase.pro.win

echo "

## NOTE: autogenerated by createProject.sh -- do not modify!!

TEMPLATE = lib

# Input
HEADERS += Reference.hpp convert.hpp wrap.hpp
HEADERS += dynamicqobject.h RDynamicQObject.hpp EventLoop.hpp

SOURCES += init.cpp EventLoop.cpp style.cpp 
SOURCES += Reference.cpp utils.cpp convert.cpp wrap.cpp 
SOURCES += Test.cpp timer.cpp
SOURCES += metaobject.cpp connect.cpp invoke.cpp props.cpp
SOURCES += dynamicqobject.cpp RDynamicQObject.cpp

INCLUDEPATH += ${R_INCLUDE_DIR}
CFLAGS += -g
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


" > qtbase.pro

if test `uname` == "Darwin" ; then
    ${QMAKE} -spec macx-g++ -o Makefile.qtbase
else 
    ${QMAKE} -o Makefile.qtbase    
fi

## if macx; do 
##     ${QMAKE} -spec macx-g++ -o Makefile.qtbase
## else



