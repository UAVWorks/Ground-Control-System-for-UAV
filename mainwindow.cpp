#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebKitWidgets/QWebView>
#include <QUrl>
#include <QString>
#include <QGenericMatrix>
#include <QTimeEdit>

#include <cstdlib>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    cp = ui->customplot;

    timer = new QTimer(this);

    initGCS();

    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);

    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(on_connect_clicked()));
    connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(on_disconnect_clicked()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMap(gpsData gd)
{
     QWebView *v = findChild<QWebView *>("mapView");

     v->load(QUrl("http://maps.googleapis.com/maps/api/staticmap?center=" + QString::number(gd.x) + "," + QString::number(gd.y) + "&zoom=17&size=400x300&maptype=satellite")); // maptype: satellite or roadmap
}

void MainWindow::initGraph()
{
    accTVec.resize(DATA_NUM);
    accXVec.resize(DATA_NUM);
    accYVec.resize(DATA_NUM);
    accZVec.resize(DATA_NUM);

    magTVec.resize(DATA_NUM);
    magXVec.resize(DATA_NUM);
    magYVec.resize(DATA_NUM);
    magZVec.resize(DATA_NUM);

    gyrTVec.resize(DATA_NUM);
    gyrXVec.resize(DATA_NUM);
    gyrYVec.resize(DATA_NUM);
    gyrZVec.resize(DATA_NUM);

    for(int i = 0; i < DATA_NUM; i++)
    {
        magTVec[i] = (double) i / (double) DATA_NUM * 10.0;
        accTVec[i] = (double) i / (double) DATA_NUM * 10.0;
        gyrTVec[i] = (double) i / (double) DATA_NUM * 10.0;

        magXVec.pop_back();
        magYVec.pop_back();
        magZVec.pop_back();
        accXVec.pop_back();
        accYVec.pop_back();
        accZVec.pop_back();
        gyrXVec.pop_back();
        gyrYVec.pop_back();
        gyrZVec.pop_back();

        magXVec.push_front(200.0);
        magYVec.push_front(150.0);
        magZVec.push_front(100.0);
        accXVec.push_front(50.0);
        accYVec.push_front(0.0);
        accZVec.push_front(-50.0);
        gyrXVec.push_front(-100.0);
        gyrYVec.push_front(-150.0);
        gyrZVec.push_front(-200.0);
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
    cp->yAxis->setRange(-GRAPH_Y_RANGE, GRAPH_Y_RANGE);

    cp->replot();
}

void MainWindow::setHeading(int angle)
{
    QLabel *l = findChild<QLabel *>("headingImage");
    QString s = PATH;
    QImage img(s + "arrowCircle.png");
    QPixmap pm = QPixmap::fromImage(img);
    QMatrix rm;

    rm.rotate(angle);
    pm = pm.transformed(rm);

    l->setPixmap(pm);
    l->show();
}

void MainWindow::setRoll(int angle)
{
    QLabel *l = findChild<QLabel *>("rollImage");
    QString s = PATH;
    QImage img(s + "arrowCircle.png");
    QPixmap pm = QPixmap::fromImage(img);
    QMatrix rm;

    rm.rotate(angle);
    pm = pm.transformed(rm);

    l->setPixmap(pm);
    l->show();
}

void MainWindow::setPitch(int angle)
{
    QLabel *l = findChild<QLabel *>("pitchImage");
    QString s = PATH;
    QImage img(s + "arrowCircle.png");
    QPixmap pm = QPixmap::fromImage(img);
    QMatrix rm;

    rm.rotate(angle);
    pm = pm.transformed(rm);

    l->setPixmap(pm);
    l->show();
}

void MainWindow::readData()
{
    QByteArray data = serial->read(30);

    int size = data.size() - 10;

    int i = 0;
    bool b = false;

    if(size == -2)
    {
        if(data[i+0] == (char) 0xfe)
        {
            if(data[i+1] == (char) 0xff)
            {
            if (data[i+2] == (char) 0x01) // Roll
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                rolFile->write(QString::number(te).toStdString().c_str());
                rolFile->write("\n");
                setRoll(te);
            }
            else if (data[i+2] == (char) 0x02) // Pitch
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                pitFile->write(QString::number(te).toStdString().c_str());
                pitFile->write("\n");
                this->setPitch(te);
            }
            else if (data[i+2] == (char) 0x03) // Yaw
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                yawFile->write(QString::number(te).toStdString().c_str());
                yawFile->write("\n");
                this->setHeading(te);
            }
            else if (data[i+2] == (char) 0x0D) // Motor 1
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot1File->write(QString::number(te).toStdString().c_str());
                mot1File->write("\n");
                ui->motorLabel_1->setNum(te);
            }
            else if (data[i+2] == (char) 0x0E) // Motor 2
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot2File->write(QString::number(te).toStdString().c_str());
                mot2File->write("\n");
                ui->motorLabel_2->setNum(te);
            }
            else if (data[i+2] == (char) 0x0F) // Motor 3
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot3File->write(QString::number(te).toStdString().c_str());
                mot3File->write("\n");
                ui->motorLabel_3->setNum(te);
            }
            else if (data[i+2] == (char) 0x10) // Motor 4
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot4File->write(QString::number(te).toStdString().c_str());
                mot4File->write("\n");
                ui->motorLabel_4->setNum(te);
            }
            else if (data[i+2] == (char) 0x11) // Motor 5
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot5File->write(QString::number(te).toStdString().c_str());
                mot5File->write("\n");
                ui->motorLabel_5->setNum(te);
            }
            else if (data[i+2] == (char) 0x12) // Motor 6
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot6File->write(QString::number(te).toStdString().c_str());
                mot6File->write("\n");
                ui->motorLabel_6->setNum(te);
            }
            else if (data[i+2] == (char) 0x04) // Acc X
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                accXFile->write(QString::number(te).toStdString().c_str());
                accXFile->write("\n");
                accXVec.pop_back();
                accXVec.push_front(te);
                cp->graph(0)->setData(accTVec, accXVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x05) // Acc Y
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                accYFile->write(QString::number(te).toStdString().c_str());
                accYFile->write("\n");
                accYVec.pop_back();
                accYVec.push_front(te);
                cp->graph(1)->setData(accTVec, accYVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x06) // Acc Z
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                accZFile->write(QString::number(te).toStdString().c_str());
                accZFile->write("\n");
                accZVec.pop_back();
                accZVec.push_front(te);
                cp->graph(2)->setData(accTVec, accZVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x07) // Gyr X
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                gyrXFile->write(QString::number(te).toStdString().c_str());
                gyrXFile->write("\n");
                gyrXVec.pop_back();
                gyrXVec.push_front(te);
                cp->graph(3)->setData(gyrTVec, gyrXVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x08) // Gyr Y
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                gyrYFile->write(QString::number(te).toStdString().c_str());
                gyrYFile->write("\n");
                gyrYVec.pop_back();
                gyrYVec.push_front(te);
                cp->graph(4)->setData(gyrTVec, gyrYVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x09) // Gyr Z
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                gyrZFile->write(QString::number(te).toStdString().c_str());
                gyrZFile->write("\n");
                gyrZVec.pop_back();
                gyrZVec.push_front(te);
                cp->graph(5)->setData(gyrTVec, gyrZVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x0A) // Mag X
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                magXFile->write(QString::number(te).toStdString().c_str());
                magXFile->write("\n");
                magXVec.pop_back();
                magXVec.push_front(te);
                cp->graph(6)->setData(magTVec, magXVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x0B) // Mag Y
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                magYFile->write(QString::number(te).toStdString().c_str());
                magYFile->write("\n");
                magYVec.pop_back();
                magYVec.push_front(te);
                cp->graph(7)->setData(magTVec, magYVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x0C) // Mag Z
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                magZFile->write(QString::number(te).toStdString().c_str());
                magZFile->write("\n");
                magZVec.pop_back();
                magZVec.push_front(te);
                cp->graph(8)->setData(magTVec, magZVec);
                cp->replot();
            }
        }
        }
    }

    while(!b && size >= i && data.size() == serial->readBufferSize())
    {
        if(data[i] == (char) 0xff)
        {
            b = true;
        }
        else
            i++;
    }

    if(b && size >= i && data.size() == serial->readBufferSize())
    {
        if(data[i+0] == (char) 0xfe)
        {
            if(data[i+1] == (char) 0xff)
            {
            if (data[i+2] == (char) 0x01) // Roll
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                rolFile->write(QString::number(te).toStdString().c_str());
                rolFile->write("\n");
                setRoll(te);
            }
            else if (data[i+2] == (char) 0x02) // Pitch
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                pitFile->write(QString::number(te).toStdString().c_str());
                pitFile->write("\n");
                this->setPitch(te);
            }
            else if (data[i+2] == (char) 0x03) // Yaw
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                yawFile->write(QString::number(te).toStdString().c_str());
                yawFile->write("\n");
                this->setHeading(te);
            }
            else if (data[i+2] == (char) 0x0D) // Motor 1
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot1File->write(QString::number(te).toStdString().c_str());
                mot1File->write("\n");
                ui->motorLabel_1->setNum(te);
            }
            else if (data[i+2] == (char) 0x0E) // Motor 2
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot2File->write(QString::number(te).toStdString().c_str());
                mot2File->write("\n");
                ui->motorLabel_2->setNum(te);
            }
            else if (data[i+2] == (char) 0x0F) // Motor 3
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot3File->write(QString::number(te).toStdString().c_str());
                mot3File->write("\n");
                ui->motorLabel_3->setNum(te);
            }
            else if (data[i+2] == (char) 0x10) // Motor 4
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot4File->write(QString::number(te).toStdString().c_str());
                mot4File->write("\n");
                ui->motorLabel_4->setNum(te);
            }
            else if (data[i+2] == (char) 0x11) // Motor 5
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot5File->write(QString::number(te).toStdString().c_str());
                mot5File->write("\n");
                ui->motorLabel_5->setNum(te);
            }
            else if (data[i+2] == (char) 0x12) // Motor 6
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                mot6File->write(QString::number(te).toStdString().c_str());
                mot6File->write("\n");
                ui->motorLabel_6->setNum(te);
            }
            else if (data[i+2] == (char) 0x04) // Acc X
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                accXFile->write(QString::number(te).toStdString().c_str());
                accXFile->write("\n");
                accXVec.pop_back();
                accXVec.push_front(te);
                cp->graph(0)->setData(accTVec, accXVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x05) // Acc Y
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                accYFile->write(QString::number(te).toStdString().c_str());
                accYFile->write("\n");
                accYVec.pop_back();
                accYVec.push_front(te);
                cp->graph(1)->setData(accTVec, accYVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x06) // Acc Z
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                accZFile->write(QString::number(te).toStdString().c_str());
                accZFile->write("\n");
                accZVec.pop_back();
                accZVec.push_front(te);
                cp->graph(2)->setData(accTVec, accZVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x07) // Gyr X
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                gyrXFile->write(QString::number(te).toStdString().c_str());
                gyrXFile->write("\n");
                gyrXVec.pop_back();
                gyrXVec.push_front(te);
                cp->graph(3)->setData(gyrTVec, gyrXVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x08) // Gyr Y
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                gyrYFile->write(QString::number(te).toStdString().c_str());
                gyrYFile->write("\n");
                gyrYVec.pop_back();
                gyrYVec.push_front(te);
                cp->graph(4)->setData(gyrTVec, gyrYVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x09) // Gyr Z
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                gyrZFile->write(QString::number(te).toStdString().c_str());
                gyrZFile->write("\n");
                gyrZVec.pop_back();
                gyrZVec.push_front(te);
                cp->graph(5)->setData(gyrTVec, gyrZVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x0A) // Mag X
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                magXFile->write(QString::number(te).toStdString().c_str());
                magXFile->write("\n");
                magXVec.pop_back();
                magXVec.push_front(te);
                cp->graph(6)->setData(magTVec, magXVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x0B) // Mag Y
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                magYFile->write(QString::number(te).toStdString().c_str());
                magYFile->write("\n");
                magYVec.pop_back();
                magYVec.push_front(te);
                cp->graph(7)->setData(magTVec, magYVec);
                cp->replot();
            }
            else if (data[i+2] == (char) 0x0C) // Mag Z
            {
                int te2 = (unsigned char) data[4+i]*256.0 + (unsigned char) data[3+i];
                float te = (float) te2 / 65536.0 + (unsigned char) data[6+i]*256.0 + (unsigned char) data[5+i];

                magZFile->write(QString::number(te).toStdString().c_str());
                magZFile->write("\n");
                magZVec.pop_back();
                magZVec.push_front(te);
                cp->graph(8)->setData(magTVec, magZVec);
                cp->replot();
            }
        }
        }
    }
}

void MainWindow::initGCS()
{
    gpsData gd;

    gd.x = 36.015307;
    gd.y = 129.320987;

    setMap(gd);
    setHeading(0);
    setRoll(0);
    setPitch(0);
    initGraph();
    ui->time->setText(QDate::currentDate().toString("yyyy.MM.dd") + " " + QTime::currentTime().toString());
}

void MainWindow::on_connect_clicked()
{
    serial->setPortName(QString("/dev/tty.SLAB_USBtoUART"));
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setReadBufferSize(30);

    if (serial->open(QIODevice::ReadOnly))
    {
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);

        fileNameBase = QDate::currentDate().toString("yyyy.MM.dd") + "." + QTime::currentTime().toString("hh.mm.ss");

        accXFile = new QFile(PATH_LOG + fileNameBase + "_accX.txt");
        accYFile = new QFile(PATH_LOG + fileNameBase + "_accY.txt");
        accZFile = new QFile(PATH_LOG + fileNameBase + "_accZ.txt");
        magXFile = new QFile(PATH_LOG + fileNameBase + "_magX.txt");
        magYFile = new QFile(PATH_LOG + fileNameBase + "_magY.txt");
        magZFile = new QFile(PATH_LOG + fileNameBase + "_magZ.txt");
        gyrXFile = new QFile(PATH_LOG + fileNameBase + "_gyrX.txt");
        gyrYFile = new QFile(PATH_LOG + fileNameBase + "_gyrY.txt");
        gyrZFile = new QFile(PATH_LOG + fileNameBase + "_gyrZ.txt");
        mot1File = new QFile(PATH_LOG + fileNameBase + "_mot1.txt");
        mot2File = new QFile(PATH_LOG + fileNameBase + "_mot2.txt");
        mot3File = new QFile(PATH_LOG + fileNameBase + "_mot3.txt");
        mot4File = new QFile(PATH_LOG + fileNameBase + "_mot4.txt");
        mot5File = new QFile(PATH_LOG + fileNameBase + "_mot5.txt");
        mot6File = new QFile(PATH_LOG + fileNameBase + "_mot6.txt");
        rolFile = new QFile(PATH_LOG + fileNameBase + "_rol.txt");
        pitFile = new QFile(PATH_LOG + fileNameBase + "_pit.txt");
        yawFile = new QFile(PATH_LOG + fileNameBase + "_yaw.txt");

        accXFile->open(QIODevice::WriteOnly);
        accYFile->open(QIODevice::WriteOnly);
        accZFile->open(QIODevice::WriteOnly);
        magXFile->open(QIODevice::WriteOnly);
        magYFile->open(QIODevice::WriteOnly);
        magZFile->open(QIODevice::WriteOnly);
        gyrXFile->open(QIODevice::WriteOnly);
        gyrYFile->open(QIODevice::WriteOnly);
        gyrZFile->open(QIODevice::WriteOnly);
        mot1File->open(QIODevice::WriteOnly);
        mot2File->open(QIODevice::WriteOnly);
        mot3File->open(QIODevice::WriteOnly);
        mot4File->open(QIODevice::WriteOnly);
        mot5File->open(QIODevice::WriteOnly);
        mot6File->open(QIODevice::WriteOnly);
        rolFile->open(QIODevice::WriteOnly);
        pitFile->open(QIODevice::WriteOnly);
        yawFile->open(QIODevice::WriteOnly);
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Fail to Connect"));
    }
}

void MainWindow::on_disconnect_clicked()
{
    serial->close();

    accXFile->close();
    accYFile->close();
    accZFile->close();
    magXFile->close();
    magYFile->close();
    magZFile->close();
    gyrXFile->close();
    gyrYFile->close();
    gyrZFile->close();
    mot1File->close();
    mot2File->close();
    mot3File->close();
    mot4File->close();
    mot5File->close();
    mot6File->close();
    rolFile->close();
    pitFile->close();
    yawFile->close();

    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
}

void MainWindow::timeUpdate()
{
    ui->time->setText(QDate::currentDate().toString("yyyy.MM.dd") + " " + QTime::currentTime().toString());
}
