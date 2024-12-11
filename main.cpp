#include "mainwindow.h"

#include <QApplication>
#include "changerecord.h"

std::mutex* mutexQ;
std::condition_variable* condition;
bool* doneCond;
std::queue<ChangeRecord>* queueChanges;
std::vector<std::vector<QGraphicsRectItem*>>* bitmapPtr;

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

    std::thread udpSender(udpSenderFunc);

    w.show();


    // udpSender.join();

    return a.exec();
}

void udpSenderFunc() {
    UdpManager udpManager((*bitmapPtr).size(), (bitmapPtr[0]).size());

    while (!(*doneCond)) {
        std::unique_lock<std::mutex> lock(*mutexQ);

        // condition->wait_for(lock, std::chrono::seconds(1),[] {
        //     return !queueChanges->empty() || (*doneCond);
        // });

        condition->wait(lock,[] {
            return !queueChanges->empty() || (*doneCond);
        });

        while (!queueChanges->empty()) {
            if(queueChanges->size() <= 255) {
                std::queue<ChangeRecord> que = (*queueChanges);
                while (!queueChanges->empty()) {
                    queueChanges->pop();
                }
                lock.unlock();

                udpManager.sendPixelVector(que);
            } else {
                auto change = queueChanges->front();
                udpManager.sendBitmap((*bitmapPtr), (*queueChanges), false);
                while(!queueChanges->empty())
                {
                    queueChanges->pop();
                }
                lock.unlock();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
