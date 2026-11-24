// Capture the actual application widgets, without adding demo code to the app.
#include <QApplication>
#include <QDir>
#include <QThread>
#include <QTextStream>
#include "mainwindow.h"

static void settle() {
    QApplication::processEvents();
    QThread::msleep(150);
    QApplication::processEvents();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    if (app.arguments().size() != 2) {
        qCritical() << "Usage: capture_screenshots <output-directory>";
        return 1;
    }
    const QDir output(QDir::cleanPath(app.arguments().at(1)));
    if (!QDir().mkpath(output.absolutePath())) return 1;

    MainWindow window;
    window.resize(1440, 900);
    window.move(40, 40);
    window.show();
    window.raise();
    window.activateWindow();
    settle();
    auto view = window.findChild<ZoomableView*>();
    if (!view) return 1;
    const auto save = [&](const QString& name) {
        settle();
        const QString path = output.absoluteFilePath(name);
        // Grab only this application's widget, even if another app is in front.
        if (!window.grab().save(path)) return false;
        qInfo().noquote() << path;
        return true;
    };
    if (!save("map-overview.png")) return 1;

    // Zoom the central area and use the same station-click handler as the user.
    const Station station = window.BeiJingSubway.getStationByName(QStringLiteral("东单"));
    view->fitInView(QRectF(4400, 2900, 3300, 2800), Qt::KeepAspectRatio);
    settle();
    const QPoint click = view->mapFromScene(station.points);
    QMouseEvent press(QEvent::MouseButtonPress, click, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(view->viewport(), &press);
    settle();
    StationPopup* popup = nullptr;
    for (auto candidate : view->findChildren<StationPopup*>())
        if (candidate->isVisible()) popup = candidate;
    if (!popup) return 1;
    // The popup is a separate native window; capture that actual widget directly.
    if (!popup->grab().save(output.absoluteFilePath("station-details.png"))) return 1;
    popup->hide();

    const Station start = window.BeiJingSubway.getStationByName(QStringLiteral("天安门西"));
    const Station end = window.BeiJingSubway.getStationByName(QStringLiteral("奥林匹克公园"));
    if (!QMetaObject::invokeMethod(&window, "handleStationButton1", Qt::DirectConnection, Q_ARG(Station, start))) return 1;
    if (!QMetaObject::invokeMethod(&window, "handleStationButton2", Qt::DirectConnection, Q_ARG(Station, end))) return 1;
    const QList<Station> route = window.BeiJingSubway.findShortestPath();
    if (route.isEmpty()) return 1;
    QRectF routeBounds(route.first().points, QSizeF(1, 1));
    for (const auto& item : route) routeBounds |= QRectF(item.points, QSizeF(1, 1));
    view->fitInView(routeBounds.adjusted(-1000, -700, 1000, 700), Qt::KeepAspectRatio);
    if (!save("route-query.png")) return 1;
    qInfo() << "Captured 3 real UI images; route intervals:" << route.size() - 1;
    return 0;
}
