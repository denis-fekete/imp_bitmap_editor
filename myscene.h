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

class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit MyScene(unsigned* gridWidth, unsigned* gridHeight,
                     unsigned* cellSize, QObject *parent = nullptr,
                     std::vector<std::vector<QGraphicsRectItem*>>* bitmap = nullptr,
                     QColor* color = nullptr,
                    std::mutex* mutex = nullptr,
                     std::condition_variable* cond = nullptr,
                     std::queue<std::pair<unsigned, unsigned>>* changes = nullptr
                     );
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
private:
    unsigned* gridWidth;
    unsigned* gridHeight;
    unsigned* cellSize;

    std::mutex* mutex;
    std::condition_variable* cond;
    std::queue<std::pair<unsigned, unsigned>>* changes;

    QColor* currentColor;
    std::vector<std::vector<QGraphicsRectItem*>>* bitmap;

    void updateCellAt(const QPointF &scenePos);
};


#endif // MYSCENE_H
