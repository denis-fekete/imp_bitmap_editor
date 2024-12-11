#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentColor = QColor();
    udpManager = new UdpManager(gridWidth, gridHeight);

    autoRefresh = false;
    scene = new MyScene(&gridWidth, &gridHeight, &cellSize, this, &bitmap, &currentColor, &mutex, &cond, &changes, &autoRefresh);

    udpManager->setAddress(ui->ipaddr_input->toPlainText());
    udpManager->setPort(ui->port_input->value());

    MyView* graphicsView = new MyView(this);
    ui->graphicsView = graphicsView;

    ui->graphicsView->setScene(scene);
    previewScene = new QGraphicsScene(this);
    ui->color_preview->setScene(previewScene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->graphicsView->setBackgroundBrush(QBrush(Qt::gray, Qt::DiagCrossPattern));

    createGrid();

}


MainWindow::~MainWindow()
{
    delete ui->graphicsView;
    delete previewScene;
    delete ui;
    delete scene;
    done = true;
    delete udpManager;
    cond.notify_one();
}

void MainWindow::createGrid()
{
    for(size_t row = 0; row < gridHeight; row++)
    {
        std::vector<QGraphicsRectItem*> line;
        for(size_t col = 0; col < gridWidth; col++)
        {
            QGraphicsRectItem* cell = new QGraphicsRectItem(col * cellSize, row * cellSize, cellSize, cellSize);
            cell->setBrush(QBrush(QColor(
                std::min(255 - (int)row * 2, 255),
                std::min((int)col * 2, 255),
                std::min(std::max(128 - (int)(row * col / 128), 0), 255)
                )));

            cell->setPen(QPen(Qt::black));

            line.push_back(cell);

            scene->addItem(cell);
        }

        bitmap.push_back(line);
    }

    // scene->setSceneRect(-(gridWidth * cellSize), -(gridHeight * cellSize), 3 * gridWidth * cellSize,  3 * gridHeight * cellSize);
    scene->setSceneRect(-100 , -100, gridWidth * cellSize + 200,  gridHeight * cellSize + 200);
}



void MainWindow::resizeEvent(QResizeEvent*)
{


#define SPACING 5

    const auto graphicalSceneW = this->window()->size().width()
                                 - ui->mainMenu->geometry().width()
                                 - SPACING;
    const auto graphicalSceneH = this->window()->size().height() - SPACING;

    ui->graphicsView->setGeometry(  ui->mainMenu->geometry().width() + SPACING,
                                    SPACING,
                                    graphicalSceneW,
                                    graphicalSceneH);
#undef SPACING
}

void MainWindow::on_btn_send_clicked()
{
    udpManager->sendBitmap(bitmap, changes, false);
}

void MainWindow::on_btn_send_2_clicked()
{
    udpManager->sendBitmap(bitmap, changes, true);
}


void MainWindow::on_btn_color_select_clicked()
{
    QColor color = QColorDialog::getColor(currentColor, this, "Select Color");
    if (color.isValid()) {
        currentColor = color;

        ui->color_preview->setBackgroundBrush(QBrush(color, Qt::SolidPattern));
    }
}


void MainWindow::on_port_input_valueChanged(int arg1)
{
    udpManager->setPort(ui->port_input->value());
}


void MainWindow::on_ipaddr_input_textChanged()
{
    udpManager->setAddress(ui->ipaddr_input->toPlainText());
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    autoRefresh = arg1;
}


void MainWindow::on_btn_fill_clicked()
{
    udpManager->sendFill(currentColor);
}

