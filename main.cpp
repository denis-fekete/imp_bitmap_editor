#include "mainwindow.h"

#include <QApplication>

std::mutex* mutexQ;
std::condition_variable* condition;
bool* doneCond;
std::queue<std::pair<unsigned, unsigned>>* queueChanges;
std::vector<std::vector<QGraphicsRectItem*>>* bitmapPtr;
UdpManager* udpMangerPtr;

void udpSenderFunc();



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.done = false;

    mutexQ = &(w.mutex);
    condition = &(w.cond);
    doneCond = &(w.done);
    queueChanges = &(w.changes);
    bitmapPtr = &(w.bitmap);
    udpMangerPtr = &(w.udpManager);

    std::thread udpSender(udpSenderFunc);

    w.show();


    // udpSender.join();

    return a.exec();
}

void udpSenderFunc() {
    while (!(*doneCond)) {
        std::unique_lock<std::mutex> lock(*mutexQ);

        condition->wait(lock, []{ return !queueChanges->empty() || (*doneCond); });

        while (!queueChanges->empty()) {
            auto change = queueChanges->front();
            lock.unlock();
            udpMangerPtr->sendSetPixel(change.first, change.second, (*bitmapPtr));
            lock.lock();
            queueChanges->pop();
        }
        lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
