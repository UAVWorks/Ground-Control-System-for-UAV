#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtCore/QtGlobal>

#include "qcustomplot.h"

#define DATA_NUM 1000
#define GRAPH_Y_RANGE 1000
#define PATH "/Users/jeongtaek/Dropbox/Development/GCS_for_UAV/Resources/"
#define PATH_LOG "/Users/jeongtaek/Dropbox/Development/GCS_for_UAV/Logs/"

#define DATASIZE 8

#define STARTBYTE 0xff
#define ROLL 0x01
#define PITCH 0x02
#define HEADING 0x03
#define MOTOR1 0x04
#define MOTOR2 0x05
#define MOTOR3 0x06
#define MOTOR4 0x07
#define MOTOR5 0x08
#define MOTOR6 0x09
#define ACCX 0x0A
#define ACCY 0x0B
#define ACCZ 0x0C
#define GYRX 0x0D
#define GYRY 0x0E
#define GYRZ 0x0F
#define MAGX 0x10
#define MAGY 0x11
#define MAGZ 0x12

namespace Ui {
class MainWindow;
}

struct gpsData
{
    double x;
    double y;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_connect_clicked();
    void on_disconnect_clicked();
    void readData();
    void timeUpdate();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QCustomPlot *cp;
    QTimer *timer;

    QString fileNameBase;

    char receiveData[4];

    QFile *accXFile;
    QFile *accYFile;
    QFile *accZFile;
    QFile *magXFile;
    QFile *magYFile;
    QFile *magZFile;
    QFile *gyrXFile;
    QFile *gyrYFile;
    QFile *gyrZFile;
    QFile *mot1File;
    QFile *mot2File;
    QFile *mot3File;
    QFile *mot4File;
    QFile *mot5File;
    QFile *mot6File;
    QFile *rolFile;
    QFile *pitFile;
    QFile *yawFile;

    QVector<double> accTVec;
    QVector<double> accXVec;
    QVector<double> accYVec;
    QVector<double> accZVec;

    QVector<double> gyrTVec;
    QVector<double> gyrXVec;
    QVector<double> gyrYVec;
    QVector<double> gyrZVec;

    QVector<double> magTVec;
    QVector<double> magXVec;
    QVector<double> magYVec;
    QVector<double> magZVec;

    void setMap(gpsData d);
    void initGCS();
    void initGraph();
    void setHeading(int angle);
    void setRoll(int angle);
    void setPitch(int angle);
};

#endif // MAINWINDOW_H
