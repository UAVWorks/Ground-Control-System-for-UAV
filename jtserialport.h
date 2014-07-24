#ifndef JTSERIALPORT_H
#define JTSERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class JTSerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit JTSerialPort(QObject *parent = 0);

signals:

public slots:

};

#endif // JTSERIALPORT_H
