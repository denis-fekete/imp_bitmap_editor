#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QColor>
#include <QWheelEvent>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "changerecord.h"

class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit MyScene(unsigned* gridWidth, unsigned* gridHeight,
                     unsigned* cellSize, QObject *parent = nullptr,
                     std::vector<std::vector<QGraphicsRectItem*>>* bitmap = nullptr,
                     QColor* color = nullptr,
                    std::mutex* mutex = nullptr,
                     std::condition_variable* cond = nullptr,
                     std::queue<ChangeRecord>* changes = nullptr,
                     bool* allowedAutoRefresh = nullptr
                     );
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
private:
    unsigned* gridWidth;
    unsigned* gridHeight;
    unsigned* cellSize;
    bool* allowedAutoRefresh;

    std::mutex* mutex;
    std::condition_variable* cond;
    std::queue<ChangeRecord>* changes;

    QColor* currentColor;
    std::vector<std::vector<QGraphicsRectItem*>>* bitmap;

    void updateCellAt(const QPointF &scenePos);
};


#endif // MYSCENE_H
