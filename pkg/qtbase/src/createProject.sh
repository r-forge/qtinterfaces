
if test -z "${QMAKE}" ; then
	if test -f /usr/bin/qmake-qt4; then
		QMAKE=qmake-qt4
	else
		QMAKE=qmake
	fi
fi

## REMEMBER: any changes below should also be applied to
## qtbase.pro.win

echo "

## NOTE: autogenerated by createProject.sh -- do not modify!!

TEMPLATE = lib

# Input
HEADERS += Reference.hpp convert.hpp wrap.hpp
HEADERS += dynamicqobject.h RDynamicQObject.hpp EventLoop.hpp

HEADERS += ClassFactory.hpp
HEADERS += Class.hpp SmokeClass.hpp MocClass.hpp RClass.hpp
HEADERS += Method.hpp ForeignMethod.hpp
HEADERS += SmokeMethod.hpp RMethod.hpp MocMethod.hpp MocInvokable.hpp
HEADERS += DynamicBinding.hpp MocDynamicBinding.hpp  
HEADERS += MethodCall.hpp type-handlers.hpp TypeHandler.hpp
HEADERS += MocStack.hpp SmokeStack.hpp SmokeType.hpp
HEADERS += RQtModule.hpp RSmokeBinding.hpp SmokeList.hpp
HEADERS += SmokeObject.hpp ObjectTable.hpp InstanceObjectTable.hpp


SOURCES += init.cpp EventLoop.cpp style.cpp 
SOURCES += Reference.cpp utils.cpp convert.cpp wrap.cpp 
SOURCES += Test.cpp timer.cpp
SOURCES += metaobject.cpp connect.cpp invoke.cpp props.cpp
SOURCES += dynamicqobject.cpp RDynamicQObject.cpp

SOURCES += ClassFactory.cpp
SOURCES += Class.cpp SmokeClass.cpp MocClass.cpp RClass.cpp classes.cpp
SOURCES += ForeignMethod.cpp
SOURCES += SmokeMethod.cpp RMethod.cpp MocMethod.cpp
SOURCES += DynamicBinding.cpp MocDynamicBinding.cpp  
SOURCES += MethodCall.cpp type-handlers.cpp
SOURCES += MocStack.cpp
SOURCES += RQtModule.cpp module.cpp RSmokeBinding.cpp SmokeList.cpp
SOURCES += SmokeObject.cpp ObjectTable.cpp InstanceObjectTable.cpp smoke.cpp 

INCLUDEPATH += ${R_INCLUDE_DIR} ../local/include
CFLAGS += -g
LIBS += -L../local/lib -lsmokeqt

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



