#include "myscene.h"
#include <QGraphicsSceneMouseEvent>
MyScene::MyScene(unsigned* gridWidth, unsigned* gridHeight,
        unsigned* cellSize, QObject *parent,
        std::vector<std::vector<QGraphicsRectItem*>>* bitmap,
        QColor* color,
        std::mutex* mutex,
        std::condition_variable* cond,
        std::queue<std::pair<unsigned, unsigned>>* changes
        ) : QGraphicsScene(parent), gridWidth(gridWidth), gridHeight(gridHeight),
    cellSize(cellSize), bitmap(bitmap), currentColor(color), mutex(mutex), cond(cond), changes(changes)
{}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        updateCellAt(event->scenePos());
    }
}


void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() & Qt::RightButton) {
        updateCellAt(event->scenePos());
    }
}

void MyScene::updateCellAt(const QPointF &scenePos)
{
    int col = static_cast<int>(scenePos.x()) / *cellSize;
    int row = static_cast<int>(scenePos.y()) / *cellSize;

    if (row >= 0 && row < *gridHeight && col >= 0 && col < *gridWidth) {
        (*bitmap)[row][col]->setBrush(QBrush(*currentColor));
        std::lock_guard<std::mutex> lock(*mutex);
        changes->push(std::make_pair(row, col));
        cond->notify_one();
    }
}
