#ifndef STATIONPOPUPUI_H
#define STATIONPOPUPUI_H

#include <QWidget>
#include"Station.h"
namespace Ui {
class StationPopupUI;
}

class StationPopup : public QWidget {
    Q_OBJECT

public:
    explicit StationPopup(QWidget* parent = nullptr);
    ~StationPopup();
    void setStation(const Station& station);
    Station getStation() const;
    // 设置站点信息
    void setStationInfo(const QString& name, const QString& details);
    //void setStationName(const QString& name);
signals:

    void button1Clicked(const Station& Newstation);
    void button2Clicked(const Station& Newstation);
    void button3Clicked();
    void button4Clicked();


private slots:

    void on_button1_clicked();
    void on_button2_clicked();
    void on_button3_clicked();
    void on_button4_clicked();

private:
    Ui::StationPopupUI* ui;  // UI类指针
    Station m_currentStation;
};

#endif // STATIONPOPUPUI_H
