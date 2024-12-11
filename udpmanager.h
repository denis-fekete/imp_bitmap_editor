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
#include <queue>
#include <mutex>

#include "changerecord.h"

class UdpManager
{
    QUdpSocket udpSocket;
    QByteArray data;
    QHostAddress serverAddress;
    quint16 serverPort = 50001;
    unsigned retryCount = 3;

    unsigned width;
    unsigned height;
public:
    UdpManager();
    UdpManager(unsigned width, unsigned height);

    void sendBitmap(std::vector<std::vector<QGraphicsRectItem*>>& bitmap, std::queue<ChangeRecord> &queue, bool fast);
    void sendRowCompressed(unsigned row, std::vector<QGraphicsRectItem*>& line);
    void sendRowUncompressed(unsigned row, std::vector<QGraphicsRectItem*>& line);
    void sendSetPixel(unsigned row, unsigned col, std::vector<std::vector<QGraphicsRectItem*>>& bitmap);
    void sendPixelVector(std::queue<ChangeRecord>& queue);
    void sendFill(QColor currentColor);

    void setAddress(const QString addr);
    void setPort(const quint16 port);
    void setRetryCount(unsigned retry);
};

#endif // UDPMANAGER_H
