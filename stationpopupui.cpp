#include "stationpopupui.h"
#include "ui_stationpopupui.h"

/*StationPopupUI::StationPopupUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StationPopupUI)
{
    ui->setupUi(this);
}

StationPopupUI::~StationPopupUI()
{
    delete ui;
}*/


StationPopup::StationPopup(QWidget* parent) : QWidget(parent), ui(new Ui::StationPopupUI) {
    ui->setupUi(this);  // 初始化UI

    // 设置窗口属性
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
   // setAttribute(Qt::WA_TranslucentBackground);
    // 使用UI设计的尺寸

    // setupUi uses connectSlotsByName; do not connect the same buttons twice.
    ui->titleLabel->setReadOnly(true);
    ui->detailsText->setReadOnly(true);
}

StationPopup::~StationPopup() {
    delete ui;
}
/*void StationPopup::setStationName(const QString& name) {
    m_station.Name = name;
}*/
void StationPopup::setStationInfo(const QString& name, const QString& details) {
    ui->titleLabel->setText(name);
    ui->detailsText->setText(details);
}

void StationPopup::on_button1_clicked() {
    emit button1Clicked(m_currentStation);
    close();
}

void StationPopup::on_button2_clicked() {
    emit button2Clicked(m_currentStation);
    close();
}

void StationPopup::setStation(const Station& station) {
    m_currentStation = station;
}

Station StationPopup::getStation() const {
    return m_currentStation;
}

void StationPopup::on_button3_clicked() {
    emit button3Clicked();
    close();
}
void StationPopup::on_button4_clicked() {
    emit button4Clicked();
    close();
}
