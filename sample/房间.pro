QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += "./glm"

INCLUDEPATH += "K:\software\glew-2.1.0\include"



LIBS += \
	Glu32.lib \
	OpenGL32.lib

SOURCES += \
    main.cpp \
    FBO.cpp \
    misc.cpp\
    model.cpp

HEADERS += \
    FBO.h \
    misc.h\
    model.h\
    Param.h