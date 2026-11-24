// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>
#include <QList>
#include <QPointF>

#include"ZoomableView.h"
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Subway BeiJingSubway;
private slots:
    void onRemoveButton3Clicked();
    void onRemoveButton4Clicked();
    void handleStationButton1(const Station& Newstation);
    void handleStationButton2(const Station& Newstation);
private:
    ZoomableView *graphicsView;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
