/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QFrame *frame_4;
    QLabel *label_2;
    QLabel *label_7;
    QFrame *frame_5;
    QLabel *label_6;
    QLabel *label_8;
    QFrame *frame_6;
    QLabel *label_4;
    QWebView *webView;
    QFrame *frame_7;
    QLabel *label_5;
    QCustomPlot *customplot;
    QFrame *frame_2;
    QFrame *frame_11;
    QLabel *label_16;
    QFrame *frame_19;
    QFrame *frame_20;
    QLabel *label_18;
    QFrame *frame_3;
    QFrame *frame_8;
    QFrame *frame_9;
    QLabel *label_3;
    QLabel *label_9;
    QFrame *frame_10;
    QLabel *label_15;
    QLabel *label;
    QFrame *frame_15;
    QLabel *label_17;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 960);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 0, 480, 938));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setGeometry(QRect(0, 0, 480, 100));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_2 = new QLabel(frame_4);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(0, 0, 480, 30));
        QFont font;
        font.setPointSize(20);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(frame_4);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(140, 40, 221, 30));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignCenter);
        frame_5 = new QFrame(frame);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setGeometry(QRect(0, 100, 480, 100));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        label_6 = new QLabel(frame_5);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(0, 0, 480, 30));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(frame_5);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(140, 50, 211, 30));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignCenter);
        frame_6 = new QFrame(frame);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setGeometry(QRect(0, 200, 480, 369));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        label_4 = new QLabel(frame_6);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(0, 0, 480, 30));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);
        webView = new QWebView(frame_6);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setGeometry(QRect(40, 50, 400, 300));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));
        frame_7 = new QFrame(frame);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setGeometry(QRect(0, 569, 480, 369));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        label_5 = new QLabel(frame_7);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(0, 0, 480, 30));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);
        customplot = new QCustomPlot(frame_7);
        customplot->setObjectName(QStringLiteral("customplot"));
        customplot->setGeometry(QRect(40, 50, 400, 300));
        customplot->setAutoFillBackground(false);
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(880, 0, 400, 938));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_11 = new QFrame(frame_2);
        frame_11->setObjectName(QStringLiteral("frame_11"));
        frame_11->setGeometry(QRect(0, 200, 400, 400));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        label_16 = new QLabel(frame_11);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(0, 0, 400, 30));
        label_16->setFont(font);
        label_16->setAlignment(Qt::AlignCenter);
        frame_19 = new QFrame(frame_2);
        frame_19->setObjectName(QStringLiteral("frame_19"));
        frame_19->setGeometry(QRect(0, 0, 400, 200));
        frame_19->setFrameShape(QFrame::StyledPanel);
        frame_19->setFrameShadow(QFrame::Raised);
        frame_20 = new QFrame(frame_2);
        frame_20->setObjectName(QStringLiteral("frame_20"));
        frame_20->setGeometry(QRect(0, 600, 400, 338));
        frame_20->setFrameShape(QFrame::StyledPanel);
        frame_20->setFrameShadow(QFrame::Raised);
        label_18 = new QLabel(frame_20);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(0, 0, 400, 30));
        label_18->setFont(font);
        label_18->setAlignment(Qt::AlignCenter);
        frame_3 = new QFrame(centralWidget);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(480, 0, 400, 938));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_8 = new QFrame(frame_3);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setGeometry(QRect(0, 0, 400, 100));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        frame_9 = new QFrame(frame_3);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setGeometry(QRect(0, 100, 400, 100));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(frame_9);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(0, 0, 400, 30));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(frame_9);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(90, 50, 211, 30));
        label_9->setFont(font);
        label_9->setAlignment(Qt::AlignCenter);
        frame_10 = new QFrame(frame_3);
        frame_10->setObjectName(QStringLiteral("frame_10"));
        frame_10->setGeometry(QRect(0, 200, 400, 400));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        label_15 = new QLabel(frame_10);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(0, 0, 400, 30));
        label_15->setFont(font);
        label_15->setAlignment(Qt::AlignCenter);
        label = new QLabel(frame_10);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 60, 282, 282));
        label->setAlignment(Qt::AlignCenter);
        frame_15 = new QFrame(frame_3);
        frame_15->setObjectName(QStringLiteral("frame_15"));
        frame_15->setGeometry(QRect(0, 600, 400, 338));
        frame_15->setFrameShape(QFrame::StyledPanel);
        frame_15->setFrameShadow(QFrame::Raised);
        label_17 = new QLabel(frame_15);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(0, 0, 400, 30));
        label_17->setFont(font);
        label_17->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 22));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GCS for UAV", 0));
        label_2->setText(QApplication::translate("MainWindow", "Current Time", 0));
        label_7->setText(QApplication::translate("MainWindow", "2014.08.29 04:59 PM ", 0));
        label_6->setText(QApplication::translate("MainWindow", "Number of Satelites", 0));
        label_8->setText(QApplication::translate("MainWindow", "4", 0));
        label_4->setText(QApplication::translate("MainWindow", "Map", 0));
        label_5->setText(QApplication::translate("MainWindow", "Sensor Status", 0));
        label_16->setText(QApplication::translate("MainWindow", "Pitch & Roll", 0));
        label_18->setText(QApplication::translate("MainWindow", "Motor Output", 0));
        label_3->setText(QApplication::translate("MainWindow", "Input Voltage", 0));
        label_9->setText(QApplication::translate("MainWindow", "3.33 V", 0));
        label_15->setText(QApplication::translate("MainWindow", "Heading", 0));
        label->setText(QApplication::translate("MainWindow", "Arrow", 0));
        label_17->setText(QApplication::translate("MainWindow", "Altitude", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
