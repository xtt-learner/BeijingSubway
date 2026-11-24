// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QStatusBar>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 设置窗口大小
    resize(1440, 960);
    setWindowTitle(QStringLiteral("北京地铁线路查询 | Beijing Subway"));
    statusBar()->showMessage(QStringLiteral("滚轮缩放 · 右键拖动 · 点击站点设置起点或终点 · 路径按最少站间区间计算"));

    // 创建绘图组件
    graphicsView = new ZoomableView(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 15000, 15000); // 画布范围
    graphicsView->setScene(scene);
    // 布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(graphicsView);
    setCentralWidget(centralWidget);
    QPixmap pixmap(":/new/prefix1/Resource/tiananmen.png");
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    QPixmap pixmap1(":/new/prefix1/Resource/airport.png");
    QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem(pixmap1);
    QPixmap pixmap2(":/new/prefix1/Resource/airport.png");
    QGraphicsPixmapItem *item2 = new QGraphicsPixmapItem(pixmap2);
    item->setPos(5410, 4400);
    item1->setPos(5750,8000);
    item2->setPos(8600,2490);
    item1->setScale(4);
    item2->setScale(4);
    scene->addItem(item);
    scene->addItem(item1);
    scene->addItem(item2);


    BeiJingSubway.GetLines();
    BeiJingSubway.DataProcessing();
    BeiJingSubway.initializeFloyd();
    for(int i=0;i<BeiJingSubway.Lines.size();i++)
    {
        BeiJingSubway.Lines[i].drawLine(scene);
    }
    scene->setSceneRect(scene->itemsBoundingRect().adjusted(-180, -180, 180, 180));
    QTimer::singleShot(0, this, [this]() {
        graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    });
    connect(graphicsView, &ZoomableView::stationButton1Clicked,
            this, &MainWindow::handleStationButton1);

    connect(graphicsView, &ZoomableView::stationButton2Clicked,
            this, &MainWindow::handleStationButton2);
    connect(graphicsView, &ZoomableView::onRemovebutton3Clicked,
            this, &MainWindow::onRemoveButton3Clicked);
    connect(graphicsView, &ZoomableView::onRemovebutton4Clicked,
            this, &MainWindow::onRemoveButton4Clicked);

}
void MainWindow::handleStationButton1(const Station& Newstation) {

    BeiJingSubway.SelectedStation.clear();
    BeiJingSubway.SelectedStation.push_back(Newstation);
    graphicsView->clearHighlight(scene);
    statusBar()->showMessage(QStringLiteral("起点：%1，请点击另一站设置终点。").arg(Newstation.Name));



}

void MainWindow::handleStationButton2(const Station& Newstation) {

    if (BeiJingSubway.SelectedStation.isEmpty()) {
        statusBar()->showMessage(QStringLiteral("请先选择起点，再设置终点。"));
        return;
    }
    if (BeiJingSubway.SelectedStation.size() > 1)
        BeiJingSubway.SelectedStation.removeLast();
    BeiJingSubway.SelectedStation.push_back(Newstation);
    QList<Station> shortestPath=BeiJingSubway.findShortestPath();


    graphicsView->highlightPath(shortestPath,scene);
    if (shortestPath.isEmpty()) {
        statusBar()->showMessage(QStringLiteral("当前数据中未找到可达路线。"));
    } else {
        statusBar()->showMessage(QStringLiteral("%1 → %2 | %3 个站间区间 | 绿色为起点，红色为终点")
            .arg(shortestPath.first().Name, shortestPath.last().Name)
            .arg(shortestPath.size() - 1));
    }


}
void MainWindow::onRemoveButton3Clicked() {
    if (!BeiJingSubway.SelectedStation.isEmpty()){
        BeiJingSubway.SelectedStation.clear();
    }

    graphicsView->clearHighlight(scene);
    statusBar()->showMessage(QStringLiteral("路线已清除，请点击站点设置起点。"));

}
void MainWindow::onRemoveButton4Clicked()
{
    if (BeiJingSubway.SelectedStation.size() > 1)
        BeiJingSubway.SelectedStation.removeLast();
    graphicsView->clearHighlight(scene);
    statusBar()->showMessage(BeiJingSubway.SelectedStation.isEmpty()
        ? QStringLiteral("请点击站点设置起点。")
        : QStringLiteral("起点：%1，请重新选择终点。").arg(BeiJingSubway.SelectedStation.first().Name));
}
MainWindow::~MainWindow() {}
