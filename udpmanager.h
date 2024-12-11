#ifndef UDPMANAGER_H
#define UDPMANAGER_H

#include <QUdpSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <vector>
#include <QBrush>
#include <QColor>

class UdpManager
{
    QUdpSocket udpSocket;
    QByteArray data;
    QHostAddress serverAddress;
    quint16 serverPort = 50001;

public:
    UdpManager();

    void sendBitmap(unsigned width, unsigned height, std::vector<std::vector<QGraphicsRectItem*>>& bitmap);
    void sendRowCompressed(unsigned row, std::vector<QGraphicsRectItem*>& line);
    void sendRowUncompressed(unsigned row, std::vector<QGraphicsRectItem*>& line);
    void sendSetPixel(unsigned row, unsigned col, std::vector<std::vector<QGraphicsRectItem*>>& bitmap);

    void setAddress(const QString addr);
    void setPort(const quint16 port);
};

#endif // UDPMANAGER_H
