#-------------------------------------------------
#
# Project created by QtCreator 2018-03-01T16:15:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = polyline
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    coordinates.cpp \
    route.cpp \
    polyline.cpp \
    command/addroutecommand.cpp \
    command/addwaypointcommand.cpp \
    command/deleteroutecommand.cpp \
    command/deletewaypointcommand.cpp \
    command/editwaypointcommand.cpp \
    routemanager.cpp

HEADERS += \
        mainwindow.h \
    coordinates.h \
    route.h \
    polyline.h \
    command/addroutecommand.h \
    command/addwaypointcommand.h \
    command/deleteroutecommand.h \
    command/deletewaypointcommand.h \
    command/editwaypointcommand.h \
    routemanager.h

FORMS += \
        mainwindow.ui
