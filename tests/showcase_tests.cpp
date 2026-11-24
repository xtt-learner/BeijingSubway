#include <QtTest>
#include <QDir>
#include <QTemporaryDir>
#include <QPushButton>
#include <QTextEdit>
#include <QScrollBar>
#include <QPointer>
#include "mainwindow.h"

class ShowcaseTests : public QObject {
    Q_OBJECT
private slots:
    void resourcesAreAvailableWithoutSourceDirectory() {
        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        const QString originalDirectory = QDir::currentPath();
        QVERIFY(QDir::setCurrent(directory.path()));
        const bool dataExists = QFile::exists(":/data/Line1.txt");
        const bool imageLoads = !QPixmap(":/new/prefix1/Resource/airport.png").isNull();
        Subway subway;
        subway.GetLines();
        const QString firstStation = subway.Lines.first().Stations.isEmpty()
            ? QString() : subway.Lines.first().Stations.first().Name;
        QDir::setCurrent(originalDirectory);
        QVERIFY2(dataExists, "Line data must be bundled with the executable");
        QVERIFY2(imageLoads, "Airport image must be bundled with the executable");
        QCOMPARE(firstStation, QStringLiteral("古城"));
    }

    void chineseDataAndAllDescriptionsLoad() {
        Subway subway;
        subway.GetLines();
        QCOMPARE(subway.Lines.size(), 28);
        QVERIFY(!subway.Lines.first().Stations.isEmpty());
        QCOMPARE(subway.Lines.first().Stations.first().Name, QStringLiteral("古城"));
        subway.DataProcessing();
        for (const auto& line : subway.Lines) {
            QVERIFY(!line.Stations.isEmpty());
            QVERIFY2(!line.Description.trimmed().isEmpty(), qPrintable(line.name));
            QVERIFY(!line.Description.contains(QChar::ReplacementCharacter));
        }
    }

    void reloadingDoesNotDuplicateStations() {
        Subway subway;
        subway.GetLines();
        const int count = subway.Lines.first().Stations.size();
        QVERIFY(count > 0);
        subway.GetLines();
        QCOMPARE(subway.Lines.first().Stations.size(), count);
    }

    void shortestRouteHasExpectedStations() {
        Subway subway;
        subway.GetLines();
        subway.initializeFloyd();
        subway.SelectedStation << subway.getStationByName(QStringLiteral("天安门西"))
                               << subway.getStationByName(QStringLiteral("王府井"));
        QStringList names;
        for (const auto& station : subway.findShortestPath()) names << station.Name;
        QCOMPARE(names, QStringList({QStringLiteral("天安门西"), QStringLiteral("天安门东"), QStringLiteral("王府井")}));
    }

    void startButtonEmitsExactlyOnce() {
        StationPopup popup;
        int count = 0;
        connect(&popup, &StationPopup::button1Clicked, [&count](const Station&) { ++count; });
        auto button = popup.findChild<QPushButton*>("button1");
        QVERIFY(button);
        button->click();
        QCOMPARE(count, 1);
    }

    void otherPopupButtonsEmitExactlyOnce() {
        StationPopup popup;
        int endCount = 0, clearCount = 0, deleteCount = 0;
        connect(&popup, &StationPopup::button2Clicked, [&endCount](const Station&) { ++endCount; });
        connect(&popup, &StationPopup::button3Clicked, [&clearCount]() { ++clearCount; });
        connect(&popup, &StationPopup::button4Clicked, [&deleteCount]() { ++deleteCount; });
        popup.findChild<QPushButton*>("button2")->click();
        popup.findChild<QPushButton*>("button3")->click();
        popup.findChild<QPushButton*>("button4")->click();
        QCOMPARE(endCount, 1);
        QCOMPARE(clearCount, 1);
        QCOMPARE(deleteCount, 1);
    }

    void selectingStartReleasesPopup() {
        QPointer<StationPopup> popup = new StationPopup;
        popup->show();
        popup->findChild<QPushButton*>("button1")->click();
        QApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
        const bool released = popup.isNull();
        if (popup) delete popup;
        QVERIFY2(released, "Completed station popups should be released, not retained invisibly");
    }

    void endpointSelectionReplacesPreviousQuery() {
        MainWindow window;
        const Station start = window.BeiJingSubway.getStationByName(QStringLiteral("天安门西"));
        const Station end = window.BeiJingSubway.getStationByName(QStringLiteral("王府井"));
        const Station replacement = window.BeiJingSubway.getStationByName(QStringLiteral("东单"));
        const auto select = [&](const char* slot, const Station& station) {
            return QMetaObject::invokeMethod(&window, slot, Qt::DirectConnection, Q_ARG(Station, station));
        };
        QVERIFY(select("handleStationButton2", end));
        QVERIFY(window.BeiJingSubway.SelectedStation.isEmpty());
        QVERIFY(select("handleStationButton1", start));
        QVERIFY(select("handleStationButton2", end));
        QVERIFY(select("handleStationButton2", replacement));
        QCOMPARE(window.BeiJingSubway.SelectedStation.size(), 2);
        QCOMPARE(window.BeiJingSubway.SelectedStation.last().Name, replacement.Name);
        QVERIFY(select("handleStationButton1", replacement));
        QCOMPARE(window.BeiJingSubway.SelectedStation.size(), 1);
        QCOMPARE(window.BeiJingSubway.SelectedStation.first().Name, replacement.Name);
    }

    void deletingEndKeepsSingleStartAndClearsRoute() {
        MainWindow window;
        const Station start = window.BeiJingSubway.getStationByName(QStringLiteral("天安门西"));
        const Station end = window.BeiJingSubway.getStationByName(QStringLiteral("王府井"));
        QVERIFY(QMetaObject::invokeMethod(&window, "handleStationButton1", Qt::DirectConnection, Q_ARG(Station, start)));
        QVERIFY(QMetaObject::invokeMethod(&window, "onRemoveButton4Clicked", Qt::DirectConnection));
        QCOMPARE(window.BeiJingSubway.SelectedStation.size(), 1);
        QVERIFY(QMetaObject::invokeMethod(&window, "handleStationButton2", Qt::DirectConnection, Q_ARG(Station, end)));
        QCOMPARE(window.BeiJingSubway.SelectedStation.size(), 2);
        QVERIFY(QMetaObject::invokeMethod(&window, "onRemoveButton4Clicked", Qt::DirectConnection));
        QCOMPARE(window.BeiJingSubway.SelectedStation.size(), 1);
        QCOMPARE(window.BeiJingSubway.SelectedStation.first().Name, start.Name);
        QVERIFY(QMetaObject::invokeMethod(&window, "onRemoveButton3Clicked", Qt::DirectConnection));
        QCOMPARE(window.BeiJingSubway.SelectedStation.size(), 0);
    }

    void clickingStationShowsItsOwnLine_data() {
        QTest::addColumn<QString>("stationName");
        QTest::addColumn<QString>("lineName");
        QTest::newRow("first-line") << QStringLiteral("古城") << QStringLiteral("1号线");
        QTest::newRow("last-line") << QStringLiteral("四道桥") << QStringLiteral("S1 线");
    }

    void clickingStationShowsItsOwnLine() {
        QFETCH(QString, stationName);
        QFETCH(QString, lineName);
        MainWindow window;
        window.show();
        QTest::qWait(100);
        auto view = window.findChild<ZoomableView*>();
        QVERIFY(view);
        const Station station = window.BeiJingSubway.getStationByName(stationName);
        view->resetTransform();
        view->centerOn(station.points);
        QTest::mouseClick(view->viewport(), Qt::LeftButton, Qt::NoModifier, view->mapFromScene(station.points));
        auto popup = view->findChild<StationPopup*>();
        QVERIFY(popup);
        auto details = popup->findChild<QTextEdit*>("detailsText");
        QVERIFY(details);
        QVERIFY(details->toPlainText().contains(lineName));
        QVERIFY(!details->toPlainText().isEmpty());
    }

    void rightDragPansZoomedMap() {
        MainWindow window;
        window.show();
        QTest::qWait(100);
        auto view = window.findChild<ZoomableView*>();
        QVERIFY(view);
        view->scale(3, 3);
        const int previous = view->horizontalScrollBar()->value();
        const QPoint from(500, 400), to(600, 400);
        QTest::mousePress(view->viewport(), Qt::RightButton, Qt::NoModifier, from);
        QMouseEvent move(QEvent::MouseMove, to, Qt::NoButton, Qt::RightButton, Qt::NoModifier);
        QApplication::sendEvent(view->viewport(), &move);
        QTest::mouseRelease(view->viewport(), Qt::RightButton, Qt::NoModifier, to);
        QCOMPARE(view->horizontalScrollBar()->value(), previous - 100);
    }
};

QTEST_MAIN(ShowcaseTests)
#include "showcase_tests.moc"
