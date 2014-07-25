#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define DATA_NUM 1000
#define PATH "/Users/jeongtaek/Dropbox/Development/GCS_for_UAV/Resources/"

namespace Ui {
class MainWindow;
}

struct gpsData
{
    double x;
    double y;
    double z;
};

struct accData
{
    unsigned int accX;
    unsigned int accY;
    unsigned int accZ;
};

struct gyrData
{
    unsigned int gyrX;
    unsigned int gyrY;
    unsigned int gyrZ;
};

struct magData
{
    unsigned int magX;
    unsigned int magY;
    unsigned int magZ;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void selectMap(gpsData d);
    void initGraph(QVector<double> accXVec, QVector<accData> accYVec, QVector<double> gyrXVec, QVector<gyrData> gyrYVec, QVector<double> magXVec, QVector<magData> magYVec);
  //  void replotGraph(QVector<double> accXVec, QVector<accData> accYVec, QVector<double> gyrXVec, QVector<gyrData> gyrYVec, QVector<double> magXVec, QVector<magData> magYVec);
  //  void drawAccGraph(accData ad);
   // void drawGyrGraph(gyrData gd);
  //  void drawMagGraph(magData md);
    void setHeading();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
