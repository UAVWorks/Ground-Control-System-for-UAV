#include "jtserialport.h"

JTSerialPort::JTSerialPort(QObject *parent) :
    QSerialPort(parent)
{

}

qint64 JTSerialPort::readJTData(char *data, int* index)
{
    char raw_data[DATASIZE];
    qint64 ret = this->readData(raw_data, DATASIZE);

    if ((raw_data[0] & STARTBYTE) == 0xff)
    {
        *index = raw_data[1];
        data[0] = raw_data[2];
        data[1] = raw_data[3];
        data[2] = raw_data[4];
        data[3] = raw_data[5];
    }
    else
    {
        *index = -1;
        data = NULL;
    }

    return ret;
}
