#include "mainwindow.h"
#include "jtserialport.h"
#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <string>

void shiftData(QVector<double>);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    JTSerialPort serialPort1;
    qint64 dataSize = 7;
    qint64 maxDataSize = 4;
    char receivedData[maxDataSize];
    int* index = NULL;

    QVector<double> accTVec(DATA_NUM);
    QVector<double> accXVec(DATA_NUM);
    QVector<double> accYVec(DATA_NUM);
    QVector<double> accZVec(DATA_NUM);

    QVector<double> gyrTVec(DATA_NUM);
    QVector<double> gyrXVec(DATA_NUM);
    QVector<double> gyrYVec(DATA_NUM);
    QVector<double> gyrZVec(DATA_NUM);

    QVector<double> magTVec(DATA_NUM);
    QVector<double> magXVec(DATA_NUM);
    QVector<double> magYVec(DATA_NUM);
    QVector<double> magZVec(DATA_NUM);

    for (int i = 0; i < DATA_NUM; i++)
    {
        accTVec[i] = i / (double) DATA_NUM * 10.0;
        gyrTVec[i] = i / (double) DATA_NUM * 10.0;
        magTVec[i] = i / (double) DATA_NUM * 10.0;

        accXVec[i] = 10 + i / (double) DATA_NUM * 10.0;
        accYVec[i] = 30 + i / (double) DATA_NUM * 10.0;
        accZVec[i] = 50 + i / (double) DATA_NUM * 10.0;

        gyrXVec[i] = 90 + i / (double) DATA_NUM * 10.0;
        gyrYVec[i] = 110 + i / (double) DATA_NUM * 10.0;
        gyrZVec[i] = 130 + i / (double) DATA_NUM * 10.0;

        magXVec[i] = 170 + i / (double) DATA_NUM * 10.0;
        magYVec[i] = 190 + i / (double) DATA_NUM * 10.0;
        magZVec[i] = 210 + i / (double) DATA_NUM * 10.0;
    }

    gpsData gd;

    QMessageBox warningNotOpen;

    // Set the warning messagebox
    warningNotOpen.setWindowTitle("Connection Warning");
    warningNotOpen.setText("Connection is not established.");
    warningNotOpen.setIcon(QMessageBox::Warning);
    warningNotOpen.setStandardButtons(QMessageBox::Retry);

    QPushButton* terminateButton = warningNotOpen.addButton("Terminate", QMessageBox::ActionRole);

    // Set the specification of serial port
    serialPort1.setPortName("/dev/tty.SLAB_USBtoUART");
    serialPort1.setDataBits(JTSerialPort::Data8);
    serialPort1.setReadBufferSize(dataSize);
    serialPort1.setParity(JTSerialPort::NoParity);
    serialPort1.setBaudRate(JTSerialPort::Baud57600);
    serialPort1.setStopBits(JTSerialPort::OneStop);
    serialPort1.setFlowControl(JTSerialPort::NoFlowControl);
    serialPort1.open(QIODevice::ReadOnly);

    // Set a default GPS location
    gd.x = 36.015307;
    gd.y = 129.320987;

    for (int i = 0; i < DATA_NUM; i++)
    {
//        accXVec[i] = i/(double)DATA_NUM*10.0;
//        gyrXVec[i] = i/(double)DATA_NUM*10.0;
//        magXVec[i] = i/(double)DATA_NUM*10.0;

     //   accYVec[i].accX = 10 + i / (double) DATA_NUM * 10.0;
     //   accYVec[i].accY = 30 + i / (double) DATA_NUM * 10.0;
     //   accYVec[i].accZ = 50 + i / (double) DATA_NUM * 10.0;

     //   gyrYVec[i].gyrX = 90 + i / (double) DATA_NUM * 10.0;
     //   gyrYVec[i].gyrY = 110 + i / (double) DATA_NUM * 10.0;
     //   gyrYVec[i].gyrZ = 130 + i / (double) DATA_NUM * 10.0;

     //   magYVec[i].magX = 170 + i / (double) DATA_NUM * 10.0;
     //   magYVec[i].magY = 190 + i / (double) DATA_NUM * 10.0;
     //   magYVec[i].magZ = 210 + i / (double) DATA_NUM * 10.0;
    }

    w.initGraph(accTVec, accXVec, accYVec, accZVec, gyrTVec, gyrXVec, gyrYVec, gyrZVec, magTVec, magXVec, magYVec, magZVec);
    w.setHeading();

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
   //         while(1)
   //         {
                if(dataSize == serialPort1.readJTData(receivedData, index))
                {
                    if (*index == 0x01)
                    {

                    }
                    else if (*index == 0x02)
                    {

                    }
                    else if (*index == 0x03)
                    {

                    }
                    else if (*index == 0x04)
                    {

                    }
                    else if (*index == 0x05)
                    {

                    }
                    else if (*index == 0x06)
                    {

                    }
                    else if (*index == 0x07)
                    {

                    }
                    else if (*index == 0x08)
                    {

                    }
                    else if (*index == 0x09)
                    {

                    }
                    else if (*index == 0x0A)
                    {
                        shiftData(accXVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        accXVec[0] = dt;
                    }
                    else if (*index == 0x0B)
                    {
                        shiftData(accYVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        accYVec[0] = dt;
                    }
                    else if (*index == 0x0C)
                    {
                        shiftData(accZVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        accZVec[0] = dt;
                    }
                    else if (*index == 0x0D)
                    {
                        shiftData(gyrXVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        gyrXVec[0] = dt;
                    }
                    else if (*index == 0x0E)
                    {
                        shiftData(gyrYVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        gyrYVec[0] = dt;
                    }
                    else if (*index == 0x0F)
                    {
                        shiftData(gyrZVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        gyrZVec[0] = dt;

                    }
                    else if (*index == 0x10)
                    {
                        shiftData(magXVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        magXVec[0] = dt;
                    }
                    else if (*index == 0x11)
                    {
                        shiftData(magYVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        magYVec[0] = dt;
                    }
                    else if (*index == 0x12)
                    {
                        shiftData(magZVec);

                        int te = 16 * 16 * 16 * receivedData[3] + 16 * 16 * receivedData[2] + 16 * receivedData[1] + receivedData[0];

                        float dt = (float)te / 256.0;

                        magZVec[0] = dt;
                    }

                    for(int i = DATA_NUM - 1; i > 0; i--)
                    {
                        accYVec[i] = accYVec[i-1];
                        gyrYVec[i] = gyrYVec[i-1];
                        magYVec[i] = magYVec[i-1];
                    }

                    // Save accerlerometer, gyroscope, magnetometer data

                    w.initGraph(accTVec, accXVec, accYVec, accZVec, gyrTVec, gyrXVec, gyrYVec, gyrZVec, magTVec, magXVec, magYVec, magZVec);
                }
      //      }
        }
    }

    return a.exec();
}

void shiftData(QVector<double> av)
{
    for(int i = DATA_NUM - 1; i > 0; i--)
    {
        av[i] = av[i-1];
    }
}
