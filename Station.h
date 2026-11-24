#ifndef STATION_H
#define STATION_H
#include<iostream>
#include <QWidget>
#include <QVector>
#include <QString>
#include <QMenu>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>
#include <QList>
#include <QPointF>
#include <QPair>
#include <QMap>
class Station
{
public:
    QString Name;
    QPointF points;
    QMap<int,int>Messages;
    QVector<int>LineNumber;
};




#endif // STATION_H
