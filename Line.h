#ifndef LINE_H
#define LINE_H
#include"Station.h"
#include <QPainterPath>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPointF>
#include <QString>
#include <QVector>
struct Turn
{
    Station PreStation;
    QPointF points;
};

class Line
{
public:
    QString name;
    int LineID;
    QString Description;
    QColor color;
    QVector<Station>Stations;
    QVector<Turn>Turns;
    void drawLine(QGraphicsScene *scene);
};



#endif // LINE_H
