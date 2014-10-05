#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T20:37:47
#
#-------------------------------------------------

QT       += core gui webkitwidgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GCS_for_UAV
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.9

SOURCES += main.cpp\
        mainwindow.cpp\
        qcustomplot.cpp

HEADERS  += mainwindow.h\
        qcustomplot.h

FORMS    += mainwindow.ui

mac: LIBS += -framework QtSerialPort
else:unix|win32: LIBS += -lQtSerialPort

mac: LIBS += -framework QtWebKitWidgets
else:unix|win32: LIBS += -lQtWebKitWidgets

