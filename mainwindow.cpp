#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebKitWidgets/QWebView>
#include <QUrl>
#include <QString>
#include <QGenericMatrix>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectMap(gpsData gd)
{
     QWebView *v = findChild<QWebView *>("webView");

     v->load(QUrl("http://maps.googleapis.com/maps/api/staticmap?center=" + QString::number(gd.x) + "," + QString::number(gd.y) + "&zoom=17&size=400x300&maptype=satellite")); // maptype: satellite or roadmap
}

void MainWindow::initGraph(QVector<double> accTVec, QVector<double> accXVec, QVector<double> accYVec, QVector<double> accZVec, QVector<double> gyrTVec, QVector<double> gyrXVec, QVector<double> gyrYVec, QVector<double> gyrZVec, QVector<double> magTVec, QVector<double> magXVec, QVector<double> magYVec, QVector<double> magZVec)
{
    QCustomPlot *cp = findChild<QCustomPlot *>("customplot");

    cp->addGraph();
    cp->addGraph();
    cp->addGraph();
    cp->addGraph();
    cp->addGraph();
    cp->addGraph();
    cp->addGraph();
    cp->addGraph();
    cp->addGraph();

    cp->graph(0)->setPen(QPen(QColor(255, 0, 0, 255)));
    cp->graph(1)->setPen(QPen(QColor(255, 0, 0, 170)));
    cp->graph(2)->setPen(QPen(QColor(255, 0, 0, 100)));

    cp->graph(3)->setPen(QPen(QColor(0, 255, 0, 255)));
    cp->graph(4)->setPen(QPen(QColor(0, 255, 0, 170)));
    cp->graph(5)->setPen(QPen(QColor(0, 255, 0, 100)));

    cp->graph(6)->setPen(QPen(QColor(0, 0, 255, 255)));
    cp->graph(7)->setPen(QPen(QColor(0, 0, 255, 170)));
    cp->graph(8)->setPen(QPen(QColor(0, 0, 255, 100)));

    cp->graph(0)->setData(accTVec, accXVec);
    cp->graph(1)->setData(accTVec, accYVec);
    cp->graph(2)->setData(accTVec, accZVec);

    cp->graph(3)->setData(gyrTVec, gyrXVec);
    cp->graph(4)->setData(gyrTVec, gyrYVec);
    cp->graph(5)->setData(gyrTVec, gyrZVec);

    cp->graph(6)->setData(magTVec, magXVec);
    cp->graph(7)->setData(magTVec, magYVec);
    cp->graph(8)->setData(magTVec, magZVec);

    cp->xAxis->setLabel("time");
    cp->yAxis->setLabel("value");

    cp->xAxis->setRange(0, 10);
    cp->yAxis->setRange(0, 260);

    cp->xAxis->setRangeReversed(true);
    cp->replot();
}
/*
void MainWindow::replotGraph(QVector<double> accXVec, QVector<accData> accYVec, QVector<double> gyrXVec, QVector<gyrData> gyrYVec, QVector<double> magXVec, QVector<magData> magYVec)
{

}

void MainWindow::drawAccGraph(accData ad)
{
     QCustomPlot *cp = findChild<QCustomPlot *>("customplot");
}

void MainWindow::drawGyrGraph(gyrData gd)
{
     QCustomPlot *cp = findChild<QCustomPlot *>("customplot");
}

void MainWindow::drawMagGraph(magData md)
{
     QCustomPlot *cp = findChild<QCustomPlot *>("customplot");
}
*/

void MainWindow::setHeading()
{
    QLabel *l = findChild<QLabel *>("label");
    QString s = PATH;
    QImage img(s + "arrowCircle.png");
    QPixmap pm = QPixmap::fromImage(img);
    QMatrix rm;
    int angle = 180;

    rm.rotate(angle);
    pm = pm.transformed(rm);

    l->setPixmap(pm);
//    l->adjustSize();
  //  l->resize(l->pixmap()->size());
//    l->move(60 + 141 - 141*cos(angle/180*3.141592), 60-141*sin(angle/180*3.141592));
    l->show();
}
