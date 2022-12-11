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
    QByteArray text;
    void* data;
};

#endif // PACKET_H
