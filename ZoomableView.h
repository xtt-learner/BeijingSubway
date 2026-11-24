#ifndef ZOOMABLEVIEW_H
#define ZOOMABLEVIEW_H
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>
#include<QMessageBox>
#include"Subway.h"
#include"stationpopupui.h"

// 自定义可缩放的视图类
class ZoomableView : public QGraphicsView {
    Q_OBJECT
public:
    ZoomableView(QWidget* parent = nullptr);
    void setDragSensitivity(qreal sensitivity);
    void highlightPath(const QList<Station>& path,QGraphicsScene *scene);
    void clearHighlight(QGraphicsScene *scene);
    void drawStartEndMarkers(const QPointF& startPos, const QPointF& endPos,QGraphicsScene *scene);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool event(QEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
signals:

    void stationButton1Clicked(const Station& Newstation);
    void stationButton2Clicked(const Station& Newstation);
    void onRemovebutton3Clicked();
    void onRemovebutton4Clicked();
    void clickedOutside();


private:
    bool m_dragging;
    QPoint m_lastPos;
    qreal m_dragSensitivity;
    Subway m_subway;
    QGraphicsItemGroup* m_highlightedPath;
    QMap<QString, QGraphicsItem*> m_stationItems;
    QList<QGraphicsItem*> m_highlightedItems;
};
#endif // ZOOMABLEVIEW_H
