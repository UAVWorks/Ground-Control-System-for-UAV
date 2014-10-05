#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebKitWidgets/QWebView>
#include <QUrl>
#include <QString>
#include <QGenericMatrix>
#include <QTimeEdit>


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

    cp->xAxis->setRangeReversed(true);
    cp->replot();
}

void MainWindow::setGraph(QVector<double> accTVec, QVector<double> accXVec, QVector<double> accYVec, QVector<double> accZVec, QVector<double> gyrTVec, QVector<double> gyrXVec, QVector<double> gyrYVec, QVector<double> gyrZVec, QVector<double> magTVec, QVector<double> magXVec, QVector<double> magYVec, QVector<double> magZVec)
{

}

void MainWindow::setHeading(int angle)
{
    QLabel *l = findChild<QLabel *>("label");
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

    if(size == -3)
    {
        if(data[0] == (char) 0xff)
        {
            if (data[1] == (char) 0x01)
            {
                ui->motorLabel_1->setNum(123);
            }
            else if (data[1] == (char) 0x02)
            {

            }
            else if (data[1] == (char) 0x03)
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                this->setHeading(te);
            }
            else if (data[1] == (char) 0x04)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                ui->motorLabel_1->setNum(te);
            }
            else if (data[1] == (char) 0x05)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                ui->motorLabel_2->setNum(te);
            }
            else if (data[1] == (char) 0x06)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                ui->motorLabel_3->setNum(te);
            }
            else if (data[1] == (char) 0x07)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                ui->motorLabel_4->setNum(te);
            }
            else if (data[1] == (char) 0x08)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                ui->motorLabel_5->setNum(te);
            }
            else if (data[1] == (char) 0x09)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                ui->motorLabel_6->setNum(te);
            }
            else if (data[1] == (char) 0x0A)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                accXVec.pop_back();
                accXVec.push_front(dt);
                cp->graph(0)->setData(accTVec, accXVec);
                cp->replot();
            }
            else if (data[1] == (char) 0x0B)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                accYVec.pop_back();
                accYVec.push_front(dt);
                cp->graph(1)->setData(accTVec, accYVec);
                cp->replot();
            }
            else if (data[1] == (char) 0x0C)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                accZVec.pop_back();
                accZVec.push_front(dt);
                cp->graph(2)->setData(accTVec, accZVec);
                cp->replot();
            }
            else if (data[1] == (char) 0x0D)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                gyrXVec.pop_back();
                gyrXVec.push_front(dt);
                cp->graph(3)->setData(gyrTVec, gyrXVec);
                cp->replot();
            }
            else if (data[1] == (char) 0x0E)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                gyrYVec.pop_back();
                gyrYVec.push_front(dt);
                cp->graph(4)->setData(gyrTVec, gyrYVec);
                cp->replot();
            }
            else if (data[1] == (char) 0x0F)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                gyrZVec.pop_back();
                gyrZVec.push_front(dt);
                cp->graph(5)->setData(gyrTVec, gyrZVec);
                cp->replot();
            }
            else if (data[1] == (char) 0x10)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                magXVec.pop_back();
                magXVec.push_front(dt);
                cp->graph(6)->setData(magTVec, magXVec);
                cp->replot();
            }
            else if (data[1] == (char) 0x11)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                magYVec.pop_back();
                magYVec.push_front(dt);
                cp->graph(7)->setData(magTVec, magYVec);
                cp->replot();
            }
            else if (data[1] == (char) 0x12)
            {
                int te = 16 * 16 * 16 * data[2] + 16 * 16 * data[3] + 16 * data[4] + data[5];

                float dt = (float)te / 256.0;

                magZVec.pop_back();
                magZVec.push_front(dt);
                cp->graph(8)->setData(magTVec, magZVec);
                cp->replot();
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
        if(data[i+0] == (char) 0xff)
        {
            if (data[i+1] == (char) 0x01) // Roll
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                ui->motorLabel_1->setNum(te);
            }
            else if (data[i+1] == (char) 0x02) // Pitch
            {

            }
            else if (data[i+1] == (char) 0x03) // Yaw
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                this->setHeading(te);
            }
            else if (data[i+1] == (char) 0x04) // Motor 1
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                ui->motorLabel_1->setNum(te);
            }
            else if (data[i+1] == (char) 0x05) // Motor 2
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                ui->motorLabel_2->setNum(te);
            }
            else if (data[i+1] == (char) 0x06) // Motor 3
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                ui->motorLabel_3->setNum(te);
            }
            else if (data[i+1] == (char) 0x07) // Motor 4
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                ui->motorLabel_4->setNum(te);
            }
            else if (data[i+1] == (char) 0x08) // Motor 5
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                ui->motorLabel_5->setNum(te);
            }
            else if (data[i+1] == (char) 0x09) // Motor 6
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                ui->motorLabel_6->setNum(te);
            }
            else if (data[i+1] == (char) 0x0A) // Acc X
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                accXVec.pop_back();
                accXVec.push_front(dt);
                cp->graph(0)->setData(accTVec, accXVec);
                cp->replot();
            }
            else if (data[i+1] == (char) 0x0B) // Acc Y
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                accYVec.pop_back();
                accYVec.push_front(dt);
                cp->graph(1)->setData(accTVec, accYVec);
                cp->replot();
            }
            else if (data[i+1] == (char) 0x0C) // Acc Z
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                accZVec.pop_back();
                accZVec.push_front(dt);
                cp->graph(2)->setData(accTVec, accZVec);
                cp->replot();
            }
            else if (data[i+1] == (char) 0x0D) // Gyr X
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                gyrXVec.pop_back();
                gyrXVec.push_front(dt);
                cp->graph(3)->setData(gyrTVec, gyrXVec);
                cp->replot();
            }
            else if (data[i+1] == (char) 0x0E) // Gyr Y
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                gyrYVec.pop_back();
                gyrYVec.push_front(dt);
                cp->graph(4)->setData(gyrTVec, gyrYVec);
                cp->replot();
            }
            else if (data[i+1] == (char) 0x0F) // Gyr Z
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                gyrZVec.pop_back();
                gyrZVec.push_front(dt);
                cp->graph(5)->setData(gyrTVec, gyrZVec);
                cp->replot();
            }
            else if (data[i+1] == (char) 0x10) // Mag X
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                magXVec.pop_back();
                magXVec.push_front(dt);
                cp->graph(6)->setData(magTVec, magXVec);
                cp->replot();
            }
            else if (data[i+1] == (char) 0x11) // Mag Y
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                magYVec.pop_back();
                magYVec.push_front(dt);
                cp->graph(7)->setData(magTVec, magYVec);
                cp->replot();
            }
            else if (data[i+1] == (char) 0x12) // Mag Z
            {
                int te = 16 * 16 * 16 * data[i+2] + 16 * 16 * data[i+3] + 16 * data[i+4] + data[i+5];

                float dt = (float)te / 256.0;

                magZVec.pop_back();
                magZVec.push_front(dt);
                cp->graph(8)->setData(magTVec, magZVec);
                cp->replot();
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
    initGraph();
    ui->time->setText(QDate::currentDate().toString("yyyy.MM.dd") + " " + QTime::currentTime().toString());
}

void MainWindow::on_connect_clicked()
{
    serial->setPortName(QString("/dev/tty.SLAB_USBtoUART"));
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setBaudRate(QSerialPort::Baud38400);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setReadBufferSize(30);

    if (serial->open(QIODevice::ReadOnly))
    {
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Fail to Connect"));
    }
}

void MainWindow::on_disconnect_clicked()
{
    serial->close();

    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
}

void MainWindow::timeUpdate()
{
    ui->time->setText(QDate::currentDate().toString("yyyy.MM.dd") + " " + QTime::currentTime().toString());
}
