#include "udpmanager.h"
#include <math.h>

UdpManager::UdpManager()
{
    // set default values
    serverAddress = QHostAddress("192.168.4.1");
    serverPort = 50001;


}

UdpManager::UdpManager(unsigned width, unsigned height) : width(width), height(height)
{
    // set default values
    serverAddress = QHostAddress("192.168.4.1");
    serverPort = 50001;


}

void UdpManager::sendBitmap(std::vector<std::vector<QGraphicsRectItem*>>& bitmap, std::queue<ChangeRecord> &queue, bool fast)
{
    qDebug() << queue.size();

    if(fast && queue.size() <= 255) {
        sendPixelVector(queue);
    } else {
        for(size_t row = 0; row < height; row++)
        {
            std::vector<QGraphicsRectItem*> line = bitmap[row];

            // UdpManager::sendRowUncompressed(row, line);
            sendRowCompressed(row, line);
        }

        while(!queue.empty())
        {
            queue.pop();
        }
    }
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
    for (unsigned i = 0; i < retryCount; i++)
    {
        if (udpSocket.waitForReadyRead(1000))
        {
            response.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(response.data(), response.size());
            break;
        }
        else
        {
            qDebug() << "No response received within timeout.";
            break;
        }
    }
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
    for (unsigned i = 0; i < retryCount; i++)
    {
        if (udpSocket.waitForReadyRead(200))
        {
            response.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(response.data(), response.size());
            break;
        }
        else
        {
            udpSocket.writeDatagram(data, serverAddress, serverPort);
        }
    }
}

void UdpManager::sendSetPixel(unsigned row, unsigned col, std::vector<std::vector<QGraphicsRectItem*>>& bitmap)
{
    data.clear();
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
    for (unsigned i = 0; i < retryCount; i++)
    {
        if (udpSocket.waitForReadyRead(200))
        {
            response.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(response.data(), response.size());
            break;
        }
        else
        {
            udpSocket.writeDatagram(data, serverAddress, serverPort);
        }
    }
}

void UdpManager::sendPixelVector(std::queue<ChangeRecord>& queue)
{
    if(queue.size() == 0) {
        return;
    }

    data.clear();
    unsigned char red, green, blue, byte0, byte1;

    data.append(0x30);

    uint16_t len = queue.size();
    unsigned char len_high = ((len & 0xFF00) >> 8);
    unsigned char len_low = (len & 0x00FF);

    data.append(len_high);
    data.append(len_low);

    uint8_t row, col;
    ChangeRecord change;

    for(unsigned i = 0; i < len; i++)
    {
        change = queue.front();
        queue.pop();
        row = change.row;
        col = change.col;


        red = std::min(change.red / 8, 31);
        green = std::min(change.green / 4, 63);
        blue = std::min(change.blue / 8, 31);

        byte0 = (blue << 3) | (green >> 3);
        byte1 = ((green & 0x07) << 5) | (red & 0x1F);

        data.append(row);
        data.append(col);
        data.append(byte0);
        data.append(byte1);
    }

    udpSocket.writeDatagram(data, serverAddress, serverPort);

    QByteArray response;
    for (unsigned i = 0; i < retryCount; i++)
    {
        if (udpSocket.waitForReadyRead(200))
        {
            response.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(response.data(), response.size());
            break;
        }
        else
        {
            udpSocket.writeDatagram(data, serverAddress, serverPort);
        }
    }
}

void UdpManager::sendFill(QColor currentColor)
{
    data.clear();
    unsigned char red, green, blue;

    data.append(0x11);

    red = currentColor.red();
    green = currentColor.green();
    blue = currentColor.blue();

    data.append(red);
    data.append(green);
    data.append(blue);

    udpSocket.writeDatagram(data, serverAddress, serverPort);

    QByteArray response;
    for (unsigned i = 0; i < retryCount; i++)
    {
        if (udpSocket.waitForReadyRead(200))
        {
            response.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(response.data(), response.size());
            break;
        }
        else
        {
            udpSocket.writeDatagram(data, serverAddress, serverPort);
        }
    }
}

void UdpManager::setAddress(const QString addr) {
    serverAddress = QHostAddress(addr);
}

void UdpManager::setPort(const quint16 port) {
    serverPort = port;
}

void UdpManager::setRetryCount(unsigned retry) {
    retryCount = retry;
}
