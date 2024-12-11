#include "udpmanager.h"
#include <math.h>

UdpManager::UdpManager() {
    // set default values
    serverAddress = QHostAddress("192.168.4.1");
    serverPort = 50001;
}

void UdpManager::sendBitmap(unsigned width, unsigned height, std::vector<std::vector<QGraphicsRectItem*>>& bitmap) {

    for(size_t row = 0; row < 128; row++)
    {
        std::vector<QGraphicsRectItem*> line = bitmap[row];

        // UdpManager::sendRowUncompressed(row, line);
        UdpManager::sendRowCompressed(row, line);
    }

    // for(size_t row = 0; row < height; row++)
    // {
    //     for(size_t col = 0; col < width; col++)
    //     {
    //         UdpManager::sendSetPixel(row, col, bitmap);
    //     }
    // }
}

void UdpManager::sendRowUncompressed(unsigned row, std::vector<QGraphicsRectItem*>& line)
{
    data.clear();

    data.append(0x22); // CMD_SET_ROW_UCMP_NORM = 0x23 - uncompressed and normalized
    data.append((unsigned char)row); // ROW - stat should be saved

    unsigned char red, green, blue, byte0, byte1;
    for(size_t i = 0; i < line.size(); i++)
    {
        const QColor& color= line[i]->brush().color();

        red = std::min(color.red() / 8, 31);
        green = std::min(color.green() / 4, 63);
        blue = std::min(color.blue() / 8, 31);

        data.append(red);
        data.append(green);
        data.append(blue);
    }

    udpSocket.writeDatagram(data, serverAddress, serverPort);

    QByteArray response;
    while (true) {
        if (udpSocket.waitForReadyRead(1000)) {  // wait 5 seconds
            response.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(response.data(), response.size());
            break;
        } else {
            // timeout occurred
            qDebug() << "No response received within timeout.";
            break;
        }
    }

    // Process the response (if needed)
    qDebug() << "Received response:" << response;
}

void UdpManager::sendRowCompressed(unsigned row, std::vector<QGraphicsRectItem*>& line)
{
    data.clear();
    unsigned char red, green, blue, byte0, byte1;

    data.append(0x23); // CMD_SET_ROW = 0x22
    data.append((unsigned char)row);

    for(size_t i = 0; i < line.size(); i++)
    {
        const QColor& color= line[i]->brush().color();
        red = std::min(color.red() / 8, 31);
        green = std::min(color.green() / 4, 63);
        blue = std::min(color.blue() / 8, 31);

        byte0 = (blue << 3) | (green >> 3);
        byte1 = ((green & 0x07) << 5) | (red & 0x1F);

        data.append(byte0);
        data.append(byte1);
    }



    udpSocket.writeDatagram(data, serverAddress, serverPort);

    QByteArray response;
    while (true) {
        if (udpSocket.waitForReadyRead(1000)) {  // wait 5 seconds
            response.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(response.data(), response.size());
            break;
        } else {
            // timeout occurred
            qDebug() << "No response received within timeout.";
            break;
        }
    }

    // Process the response (if needed)
    qDebug() << "Received response:" << response;
}

void UdpManager::sendSetPixel(unsigned row, unsigned col, std::vector<std::vector<QGraphicsRectItem*>>& bitmap)
{
    qDebug() << "Here.";
    QUdpSocket udpSocket;
    QByteArray data;
    unsigned char red, green, blue, byte0, byte1;

    data.append(0x26);
    data.append((unsigned char)row);
    data.append((unsigned char)col);

    const QColor& color= bitmap[row][col]->brush().color();
    red = std::min(color.red() / 8, 31);
    green = std::min(color.green() / 4, 63);
    blue = std::min(color.blue() / 8, 31);

    byte0 = (blue << 3) | (green >> 3);
    byte1 = ((green & 0x07) << 5) | (red & 0x1F);

    data.append(byte0);
    data.append(byte1);


    udpSocket.writeDatagram(data, serverAddress, serverPort);

    QByteArray response;
    while (true) {
        if (udpSocket.waitForReadyRead(1000)) {  // wait 5 seconds
            response.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(response.data(), response.size());
            break;
        } else {
            // timeout occurred
            qDebug() << "No response received within timeout.";
            break;
        }
    }

    // Process the response (if needed)
    qDebug() << "Received response:" << response;
}

void UdpManager::setAddress(const QString addr) {
    serverAddress = QHostAddress(addr);
}
void UdpManager::setPort(const quint16 port) {
    serverPort = port;
}
