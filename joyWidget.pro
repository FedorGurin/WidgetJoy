#-------------------------------------------------
#
# Project created by QtCreator 2016-11-17T12:14:31
#
#-------------------------------------------------

QT       += core gui network xml widgets gamepad
CONFIG   +=debug
CONFIG +=build_all
CONFIG +=no_batch


#QMAKE_CXXFLAGS += -std=c++11

TARGET = joyWidget
TEMPLATE = app

unix:DEFINES += LINUX_PLATFORM
unix:DEFINES -=WINDOWS_PLATFORM

win32:DEFINES -= LINUX_PLATFORM
win32:DEFINES += WINDOWS_PLATFORM
SOURCES += main.cpp\
        mainwindow.cpp \
    joystick.cpp \
    controller.cpp \
    keyboard.cpp \
    dispatcher.cpp

HEADERS  += mainwindow.h \
    joystick.h \
    controller.h \
    controllers.h \
    keyboard.h \
    dispatcher.h \
    controller_types.h

FORMS    += mainwindow.ui
