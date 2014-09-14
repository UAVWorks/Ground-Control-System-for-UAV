#ifndef JTSERIALPORT_H
#define JTSERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

#define DATASIZE 7

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

class JTSerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit JTSerialPort(QObject *parent = 0);
    qint64 readJTData(char* data, int* index);

signals:

public slots:

};

#endif // JTSERIALPORT_H
