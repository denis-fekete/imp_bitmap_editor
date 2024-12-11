#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include <QColorDialog>
#include <QMouseEvent>
#include <QDebug>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "udpmanager.h"
#include "myscene.h"
#include "myview.h"
#include "changerecord.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
\
    std::mutex mutex;
    std::condition_variable cond;
    bool done;
    std::queue<ChangeRecord> changes;
    std::vector<std::vector<QGraphicsRectItem*>> bitmap;
    UdpManager* udpManager;

private slots:
    void on_btn_send_clicked();

    void on_btn_color_select_clicked();

    void on_port_input_valueChanged(int arg1);

    void on_ipaddr_input_textChanged();

    void on_btn_send_2_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_btn_fill_clicked();

private:
    unsigned int gridWidth = 131;
    unsigned int gridHeight = 129;
    unsigned int cellSize = 10;
    bool autoRefresh = false;

    QColor currentColor;

    /**
     * @brief ui UI elements of application
     */
    Ui::MainWindow *ui;

    /**
     * @brief scene Pointer to the scene to which graphics items are added
     */
    MyScene* scene;

    QGraphicsScene* previewScene;

    /**
     * @brief resizeEvent Handling of window resizing
     */
    void resizeEvent(QResizeEvent*) override;

    void createGrid();
};


#endif // MAINWINDOW_H
