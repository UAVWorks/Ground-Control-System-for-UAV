#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T20:37:47
#
#-------------------------------------------------

QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GCS_for_UAV
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    jtserialport.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    jtserialport.h \
    qcustomplot.h \
    ui_mainwindow.h

FORMS    += mainwindow.ui

mac: LIBS += -framework QtSerialPort
else:unix|win32: LIBS += -lQtSerialPort

mac: LIBS += -framework QtWebKitWidgets
else:unix|win32: LIBS += -lQtWebKitWidgets

OTHER_FILES +=
