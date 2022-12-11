#ifndef PACKET_H
#define PACKET_H

#include "mainwindow.h"

enum{
    MESSAGE,
    FILE_START,
    FILE_RAW,
    FILE_END
};

class Packet{
private:
public:
    int state;
    QString text;
    QByteArray data;
};

#endif // PACKET_H
