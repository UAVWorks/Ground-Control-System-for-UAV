#include "mainwindow.h"
#include "jtserialport.h"
#include <QApplication>
#include <QMessageBox>
#include <QPushButton>

void shiftData(QVector<accData>*, QVector<gyrData>*, QVector<magData>*);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    JTSerialPort serialPort1;
    qint64 dataSize = 3;
    qint64 maxDataSize = 3;
    char receivedData[maxDataSize];

    gpsData gd;

    QMessageBox warningNotOpen;

    // Set the warning messagebox
    warningNotOpen.setWindowTitle("Connection Warning");
    warningNotOpen.setText("Connection is not established.");
    warningNotOpen.setIcon(QMessageBox::Warning);
    warningNotOpen.setStandardButtons(QMessageBox::Retry);

    QPushButton* terminateButton = warningNotOpen.addButton("Terminate", QMessageBox::ActionRole);

    // Set the specification of serial port
    serialPort1.setDataBits(QSerialPort::Data8);
    serialPort1.setReadBufferSize(dataSize);

    // Set a default GPS location
    gd.x = 36.015307;
    gd.y = 129.320987;

    QVector<accData> accYVec(DATA_NUM);
    QVector<double> accXVec(DATA_NUM);
    QVector<gyrData> gyrYVec(DATA_NUM);
    QVector<double> gyrXVec(DATA_NUM);
    QVector<magData> magYVec(DATA_NUM);
    QVector<double> magXVec(DATA_NUM);

    for (int i = 0; i < DATA_NUM; i++)
    {
        accXVec[i] = i / (double) DATA_NUM * 10.0;
        gyrXVec[i] = i / (double) DATA_NUM * 10.0;
        magXVec[i] = i / (double) DATA_NUM * 10.0;
    }

    for (int i = 0; i < DATA_NUM; i++)
    {
//        accXVec[i] = i/(double)DATA_NUM*10.0;
//        gyrXVec[i] = i/(double)DATA_NUM*10.0;
//        magXVec[i] = i/(double)DATA_NUM*10.0;

        accYVec[i].accX = 10 + i / (double) DATA_NUM * 10.0;
        accYVec[i].accY = 30 + i / (double) DATA_NUM * 10.0;
        accYVec[i].accZ = 50 + i / (double) DATA_NUM * 10.0;

        gyrYVec[i].gyrX = 90 + i / (double) DATA_NUM * 10.0;
        gyrYVec[i].gyrY = 110 + i / (double) DATA_NUM * 10.0;
        gyrYVec[i].gyrZ = 130 + i / (double) DATA_NUM * 10.0;

        magYVec[i].magX = 170 + i / (double) DATA_NUM * 10.0;
        magYVec[i].magY = 190 + i / (double) DATA_NUM * 10.0;
        magYVec[i].magZ = 210 + i / (double) DATA_NUM * 10.0;
    }

    w.initGraph(accXVec, accYVec, gyrXVec, gyrYVec, magXVec, magYVec);

    /*
    // Shift test code
    for (int i = 0; i < 100; i++)
    {
        for(int i = DATA_NUM - 1; i > 0; i--)
        {
            accYVec[i] = accYVec[i-1];
            gyrYVec[i] = gyrYVec[i-1];
            magYVec[i] = magYVec[i-1];
        }

        w.initGraph(accXVec, accYVec, gyrXVec, gyrYVec, magXVec, magYVec);
    }*/

    while(1)
    {
        w.selectMap(gd);

        // Open the serial port
        if(!serialPort1.open(QIODevice::ReadOnly))
        {
            warningNotOpen.exec();

            if(warningNotOpen.clickedButton() == terminateButton)
            {
                a.exit();

                return 1;
            }
        }
        else
        {
            while(1)
            {
                if(dataSize == serialPort1.read(receivedData, maxDataSize))
                {
                    for(int i = DATA_NUM - 1; i > 0; i--)
                    {
                        accYVec[i] = accYVec[i-1];
                        gyrYVec[i] = gyrYVec[i-1];
                        magYVec[i] = magYVec[i-1];
                    }

                    // Save accerlerometer, gyroscope, magnetometer data

                    w.initGraph(accXVec, accYVec, gyrXVec, gyrYVec, magXVec, magYVec);
                }
            }
        }
    }

    return a.exec();
}

void shiftData(QVector<accData>* av, QVector<gyrData>* gv, QVector<magData>* mv)
{
    for(int i = DATA_NUM - 1; i > 0; i--)
    {
        (*av)[i].accX = (*av)[i-1].accX;
        (*gv)[i] = (*gv)[i-1];
        (*mv)[i].magX = (*mv)[i-1].magX;
    }
}
