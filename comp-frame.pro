#-------------------------------------------------
#
# Project created by QtCreator 2019-05-27T13:12:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = comp-frame
TEMPLATE = app

QT += sql

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# Added src, include, ui directories - can break compilation
SOURCES += \
        src/advancedconfigceres.cpp \
        src/advancedconfigg2o.cpp \
        src/cereslinesearch.cpp \
        src/cerestrustregion.cpp \
        src/main.cpp \
        src/mainwindow.cpp \
        src/resultswidget.cpp

HEADERS += \
        include/advancedconfigceres.h \
        include/advancedconfigg2o.h \
        include/cereslinesearch.h \
        include/cerestrustregion.h \
        include/mainwindow.h \
        include/resultswidget.h

FORMS += \
        ui/mainwindow.ui \
        ui/resultswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
