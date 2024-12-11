#ifndef MYVIEW_H
#define MYVIEW_H


#include <QGraphicsView>
#include <QWheelEvent>

class MyView : public QGraphicsView {
    Q_OBJECT

public:
    explicit MyView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

};

#endif // MYVIEW_H
