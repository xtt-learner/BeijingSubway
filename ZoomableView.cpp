#include"ZoomableView.h"
#include <QScreen>
#include <QScrollBar>

ZoomableView::ZoomableView(QWidget* parent) : QGraphicsView(parent) {
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::NoDrag); // 初始禁用内置拖动模式
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    m_dragSensitivity = 1.0;
    m_highlightedPath = nullptr;
    m_dragging = false;
    m_subway.GetLines();
    m_subway.DataProcessing();

}
void ZoomableView::setDragSensitivity(qreal sensitivity) {

    m_dragSensitivity = qBound(0.1, sensitivity, 2.0);
}
void ZoomableView::mousePressEvent(QMouseEvent* event) {

    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos();
        QPointF scenePos = mapToScene(clickPos);

        for(int i=0;i<m_subway.Lines.size();i++)
        {
            for(QVector<Station>::iterator it=m_subway.Lines[i].Stations.begin();it !=m_subway.Lines[i].Stations.end();it++)
            {
                QRectF targetArea(it->points.x()-20, it->points.y()-20, 50, 50);
                if (targetArea.contains(scenePos)) {
                    StationPopup* popup = new StationPopup(this);


                    popup->setStation(*it);
                    QString Description;
                    bool first = true;

                    for(auto j=it->Messages.begin();j!=it->Messages.end();j++)
                    {
                        if (!first) {
                            Description.append("\n");
                        }
                        const int lineIndex = j.key() - 1;
                        if (lineIndex < 0 || lineIndex >= m_subway.Lines.size()) continue;
                        Description.append(m_subway.Lines[lineIndex].name);
                        Description.append("\n");
                        Description.append(m_subway.Lines[lineIndex].Description);
                        first = false;
                    }
                    popup->setStationInfo(
                        it->Name,
                        Description

                        );


                    QPoint globalPos = viewport()->mapToGlobal(clickPos);
                    QScreen* screen = QGuiApplication::screenAt(globalPos);
                    const QRect available = (screen ? screen : QGuiApplication::primaryScreen())->availableGeometry();
                    popup->move(qBound(available.left(), globalPos.x() - popup->width()/2,
                                       qMax(available.left(), available.right() - popup->width())),
                                qBound(available.top(), globalPos.y() - popup->height() - 20,
                                       qMax(available.top(), available.bottom() - popup->height())));
                    popup->show();

                    connect(popup, &StationPopup::button1Clicked,
                            [this, popup]() {
                                emit stationButton1Clicked(popup->getStation());
                            });

                    connect(popup, &StationPopup::button2Clicked,
                            [this, popup]() {
                                emit stationButton2Clicked(popup->getStation());
                            });
                    connect(popup, &StationPopup::button3Clicked,
                            this, &ZoomableView::onRemovebutton3Clicked);
                    connect(popup, &StationPopup::button4Clicked,
                            this, &ZoomableView::onRemovebutton4Clicked);

                    connect(this, &ZoomableView::clickedOutside,
                            popup, &StationPopup::close);




                    return;
                }
            }

        }
    }





    if (event->button() == Qt::RightButton) {
        m_dragging = true;
        m_lastPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    else {
        QGraphicsView::mousePressEvent(event);
    }
}

bool ZoomableView::event(QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        emit clickedOutside();
    }
    return QGraphicsView::event(event);
}
void ZoomableView::mouseMoveEvent(QMouseEvent* event){
    if (m_dragging && (event->buttons() & Qt::RightButton)) {
        // 计算鼠标移动距离并应用灵敏度因子
        const QPoint delta = event->pos() - m_lastPos;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - qRound(delta.x() * m_dragSensitivity));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - qRound(delta.y() * m_dragSensitivity));
        m_lastPos = event->pos();           // 更新上次位置
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}




void ZoomableView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton && m_dragging) {

        m_dragging = false;
        setCursor(Qt::ArrowCursor);
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}


void ZoomableView::wheelEvent(QWheelEvent* event)  {
    // 保存鼠标在场景中的位置
    QPointF mousePos = mapToScene(event->position().toPoint());

    // 计算缩放因子
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    // 调整视图位置，确保鼠标位置不变
    QPointF newMousePos = mapToScene(event->position().toPoint());
    QPointF delta = mousePos - newMousePos;
    translate(delta.x(), delta.y());

    event->accept();
}

void ZoomableView::highlightPath(const QList<Station>& path,QGraphicsScene *scene) {

    clearHighlight(scene);
    if (path.isEmpty()) return;

    // 2. 创建背景虚化遮罩（半透明黑色覆盖，突出路径）
    QGraphicsRectItem* mask = new QGraphicsRectItem(scene->sceneRect());
    mask->setBrush(QColor(255, 255, 255, 180));
    mask->setPen(Qt::NoPen);
    mask->setZValue(0);
    scene->addItem(mask);
    m_highlightedItems.append(mask);


    // 3. 绘制路径
    QGraphicsPathItem* pathItem = new QGraphicsPathItem();
    QPainterPath painterPath;
    painterPath.moveTo(path.first().points);  // 起点

    for (int i = 1; i < path.size(); ++i) {
        painterPath.lineTo(path[i].points);
    }



    QColor lineColor =  Qt::red;

    // 5. 设置路径样式（加粗、保留原色）
    QPen pen(lineColor, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pathItem->setPath(painterPath);
    pathItem->setPen(pen);
    pathItem->setZValue(1);  // 路径置于遮罩上
    scene->addItem(pathItem);
    m_highlightedItems.append(pathItem);

    // 6. 绘制起点/终点标识（箭头或特殊图标）
    drawStartEndMarkers(path.first().points, path.last().points,scene);

    // 7. 调整视图聚焦路径
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void ZoomableView::drawStartEndMarkers(const QPointF& startPos, const QPointF& endPos,QGraphicsScene *scene) {
    // 起点：绿色箭头
    QGraphicsPolygonItem* startArrow = new QGraphicsPolygonItem();
    QPolygonF startPoly;
    startPoly << QPointF(0, 0) << QPointF(-15, 10) << QPointF(-15, -10);
    startArrow->setPolygon(startPoly);
    startArrow->setBrush(QColor("#00FF00"));
    startArrow->setPos(startPos);
    startArrow->setZValue(2);  // 盖在路径上
    startArrow->setScale(3);
    scene->addItem(startArrow);
    m_highlightedItems.append(startArrow);

    // 终点：红色箭头（或用不同样式区分）
    QGraphicsPolygonItem* endArrow = new QGraphicsPolygonItem();
    QPolygonF endPoly;
    endPoly << QPointF(0, 0) << QPointF(15, 10) << QPointF(15, -10);
    endArrow->setPolygon(endPoly);
    endArrow->setBrush(QColor("#FF0000"));
    endArrow->setPos(endPos);
    endArrow->setZValue(2);
    endArrow->setScale(3);
    scene->addItem(endArrow);
    m_highlightedItems.append(endArrow);
}
void ZoomableView::clearHighlight(QGraphicsScene *scene) {
    foreach (QGraphicsItem* item, m_highlightedItems) {
        scene->removeItem(item);
        delete item;
    }
    m_highlightedItems.clear();
}
