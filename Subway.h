#ifndef SUBWAY_H
#define SUBWAY_H
#include"Line.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>
#include <QList>
#include <QPointF>
#include <utility>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVector>
#include <QQueue>
#include <QDebug>
#include<QColor>
#include <QMouseEvent>
#include<QMessageBox>
class Subway
{
public:
    Subway();
    QVector<Line>Lines;
    void GetLines();
    QVector<QString>Position;
    QList<Station> FindBestWay();
    QQueue<Station>SelectedStation;
    void DataProcessing();
    void initializeFloyd();
    QList<Station> findShortestPath();
    Station getStationByName(const QString& name) const;
    QMap<QString, int> StationIndex;
    QVector<QString> StationNames;
    QVector<QVector<int>> dist;
    QVector<QVector<int>> next;
    QVector<QVector<int>> lineUsed;

};


#endif // SUBWAY_H
