#include"Line.h"



void Line::drawLine(QGraphicsScene *scene)
{
    if (Stations.isEmpty()) return;
    QPen pen(color);
    pen.setWidth(20); // 线路宽度
    QPainterPath path;
    path.moveTo(Stations[0].points);
    int turnNum=0;

    for (int i = 0; i < Stations.size();i++) {
        if(i + 1 < Stations.size() && turnNum<Turns.size()&&Stations[i].Name==Turns[turnNum].PreStation.Name)
        {
            path.lineTo(Stations[i].points);
            path.moveTo(Stations[i].points);
            path.quadTo(Turns[turnNum].points, Stations[i+1].points);
            turnNum++;
        }
        else{
            path.lineTo(Stations[i].points);}

    }
    scene->addPath(path, pen);

    // 2. 绘制站点圆圈
    for (int i = 0; i < Stations.size(); ++i) {
        if(Stations[i].Name!="NULL1"&&Stations[i].Name!="NULL2"&&Stations[i].Name!="NULL3"&&Stations[i].Name!="NULL4"&&Stations[i].Name!="NULL5"&&Stations[i].Name!="NULL6"&&Stations[i].Name!="NULL7"){
        const QPointF &pos = Stations[i].points;
        QGraphicsEllipseItem *circle = scene->addEllipse(
            pos.x() - 20, pos.y() - 20, 40, 40, // 圆心(x,y)，直径10px
            QPen(Qt::black), QBrush(Qt::white)
            );
        circle->setZValue(1);}
    }

    // 3. 标注站点名称
    QFont font("黑体", 20);


    for (int i = 0; i < Stations.size(); ++i) {
    const QPointF &pos = Stations[i].points;

        if(Stations[i].Name!="NULL1"&&Stations[i].Name!="NULL2"&&Stations[i].Name!="NULL3"&&Stations[i].Name!="NULL4"&&Stations[i].Name!="NULL5"&&Stations[i].Name!="NULL6"&&Stations[i].Name!="NULL7"){
        QGraphicsTextItem *text = scene->addText(Stations[i].Name, font);
        text->setPos(pos.x() + 10, pos.y() + 10);
        text->setDefaultTextColor(Qt::darkGray);}

    }

}
