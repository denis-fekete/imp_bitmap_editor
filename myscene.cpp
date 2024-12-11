#include "myscene.h"
#include <QGraphicsSceneMouseEvent>
MyScene::MyScene(unsigned* gridWidth, unsigned* gridHeight,
        unsigned* cellSize, QObject *parent,
        std::vector<std::vector<QGraphicsRectItem*>>* bitmap,
        QColor* color,
        std::mutex* mutex,
        std::condition_variable* cond,
        std::queue<ChangeRecord>* changes,
        bool* allowedAutoRefresh
        ) : QGraphicsScene(parent), gridWidth(gridWidth), gridHeight(gridHeight),
    cellSize(cellSize), bitmap(bitmap), currentColor(color), mutex(mutex), cond(cond), changes(changes), allowedAutoRefresh(allowedAutoRefresh)
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
    if (!(event->buttons() & Qt::RightButton) && (*allowedAutoRefresh)) {
        // on release
        cond->notify_one();
    }
}

void MyScene::updateCellAt(const QPointF &scenePos)
{
    int col = static_cast<int>(scenePos.x()) / *cellSize;
    int row = static_cast<int>(scenePos.y()) / *cellSize;

    if (row >= 0 && row < *gridHeight && col >= 0 && col < *gridWidth) {
        auto cell = (*bitmap)[row][col];
        if(cell->brush().color() == *currentColor)
        {
            return; // skip if color is the same
        }

        cell->setBrush(QBrush(*currentColor));

        std::lock_guard<std::mutex> lock(*mutex);
        ChangeRecord newRecord = {.row=(unsigned char)row,
                                  .col=(unsigned char)col,
                                  .red=(unsigned char)(*currentColor).red(),
                                  .green=(unsigned char)(*currentColor).green(),
                                  .blue=(unsigned char)(*currentColor).blue()
        };
        changes->push(newRecord);
    }
}
