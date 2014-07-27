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

void MainWindow::initGraph(QVector<double> accXVec, QVector<accData> accYVec, QVector<double> gyrXVec, QVector<gyrData> gyrYVec, QVector<double> magXVec, QVector<magData> magYVec)
{
    QCustomPlot *cp = findChild<QCustomPlot *>("customplot");

    QVector<double> accYVecX(DATA_NUM);
    QVector<double> accYVecY(DATA_NUM);
    QVector<double> accYVecZ(DATA_NUM);

    QVector<double> gyrYVecX(DATA_NUM);
    QVector<double> gyrYVecY(DATA_NUM);
    QVector<double> gyrYVecZ(DATA_NUM);

    QVector<double> magYVecX(DATA_NUM);
    QVector<double> magYVecY(DATA_NUM);
    QVector<double> magYVecZ(DATA_NUM);

    for (int i = 0; i < DATA_NUM; i++)
    {
        accYVecX[i] = accYVec[i].accX;
        accYVecY[i] = accYVec[i].accY;
        accYVecZ[i] = accYVec[i].accZ;

        gyrYVecX[i] = gyrYVec[i].gyrX;
        gyrYVecY[i] = gyrYVec[i].gyrY;
        gyrYVecZ[i] = gyrYVec[i].gyrZ;

        magYVecX[i] = magYVec[i].magX;
        magYVecY[i] = magYVec[i].magY;
        magYVecZ[i] = magYVec[i].magZ;
    }

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

    cp->graph(0)->setData(accXVec, accYVecX);
    cp->graph(1)->setData(accXVec, accYVecY);
    cp->graph(2)->setData(accXVec, accYVecZ);

    cp->graph(3)->setData(gyrXVec, gyrYVecX);
    cp->graph(4)->setData(gyrXVec, gyrYVecY);
    cp->graph(5)->setData(gyrXVec, gyrYVecZ);

    cp->graph(6)->setData(magXVec, magYVecX);
    cp->graph(7)->setData(magXVec, magYVecY);
    cp->graph(8)->setData(magXVec, magYVecZ);

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
    QImage img(s + "arrow.png");
    QPixmap pm = QPixmap::fromImage(img);
    QMatrix rm;
    rm.rotate(90);

    pm = pm.transformed(rm);

    l->setPixmap(pm);
//    l->adjustSize();
    l->resize(l->pixmap()->size());
    l->show();
}
