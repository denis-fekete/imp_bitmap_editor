#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentColor = QColor();

    scene = new MyScene(&gridWidth, &gridHeight, &cellSize, this, &bitmap, &currentColor, &mutex, &cond, &changes);

    MyView* graphicsView = new MyView(this);
    ui->graphicsView = graphicsView;

    ui->graphicsView->setScene(scene);
    previewScene = new QGraphicsScene(this);
    ui->color_preview->setScene(previewScene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    // ui->graphicsView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->graphicsView->setBackgroundBrush(QBrush(Qt::gray, Qt::DiagCrossPattern));

    createGrid();

    udpManager.setAddress(ui->ipaddr_input->toPlainText());
    udpManager.setPort(ui->port_input->value());
}


MainWindow::~MainWindow()
{
    delete ui->graphicsView;
    delete previewScene;
    delete ui;
    delete scene;
    done = true;
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
            // cell->setBrush(QBrush(QColor(fmin(row * 2, 255), fmin(col * 2, 255), fmin(row * col, 255))));
            cell->setBrush(QBrush(QColor(0, fmin(col * 2, 255), 0)));
            if(col == gridWidth - 1 && row == gridHeight - 1) {
                cell->setBrush(QBrush(QColor(255, 255, 255)));
            }
            // if(col < 25) {
            //     cell->setBrush(QBrush(QColor(255, 0, 0)));
            // } else if(col < 50) {
            //     cell->setBrush(QBrush(QColor(0, 255, 0)));
            // } else if(col < 75) {
            //     cell->setBrush(QBrush(QColor(0, 0, 255)));
            // } else if(col < 100) {
            //     cell->setBrush(QBrush(QColor(255, 0, 255)));
            // } else if(col < 125) {
            //     cell->setBrush(QBrush(QColor(0, 255, 255)));
            // }

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
    udpManager.sendBitmap(gridWidth, gridHeight, bitmap);
}


void MainWindow::on_btn_save_settings_clicked()
{

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
    udpManager.setPort(ui->port_input->value());
}


void MainWindow::on_ipaddr_input_textChanged()
{
    udpManager.setAddress(ui->ipaddr_input->toPlainText());
}

