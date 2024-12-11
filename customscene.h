/**
 * @file customscene.h
 * @brief Header file and implementation of CustomScene class.
 *
 * CustomScene is
 * QGraphicsScene that can communicate with Simulator to signal it that user
 * clicked onto scene however no object was selected, there all selected
 * Robots and Obstacles should be unselected.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>

#include "QtSpecific/simulator.h"

class CustomScene : public QGraphicsScene
{
public:
    CustomScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

    void setSimulator(Simulator* simulator) { this->simulator = simulator; }
protected:
    Simulator* simulator;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsScene::mousePressEvent(event);

        // Check if an item was selected
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if (!item)
        {
            simulator->unselectActive();
        }
    }
};



#endif // CUSTOMSCENE_H
