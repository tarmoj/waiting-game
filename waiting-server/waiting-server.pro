#-------------------------------------------------
#
# Project created by QtCreator 2015-03-27T17:29:00
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = waiting-server
TEMPLATE = app

INCLUDEPATH += /usr/local/include/csound

SOURCES += main.cpp\
        waitingwindow.cpp \
    csengine.cpp \
    wsserver.cpp

HEADERS  += waitingwindow.h \
    csengine.h \
    wsserver.h

FORMS    += waitingwindow.ui

unix|win32: LIBS += -lcsnd6 -lcsound64
