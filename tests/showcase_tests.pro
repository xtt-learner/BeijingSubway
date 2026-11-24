QT += core gui widgets testlib
CONFIG += c++17 testcase console
CONFIG -= app_bundle debug_and_release
TEMPLATE = app
TARGET = showcase_tests
INCLUDEPATH += ..
SOURCES += showcase_tests.cpp \
    ../Line.cpp \
    ../Subway.cpp \
    ../ZoomableView.cpp \
    ../mainwindow.cpp \
    ../stationpopupui.cpp
HEADERS += ../Line.h ../Station.h ../Subway.h ../ZoomableView.h \
    ../mainwindow.h ../stationpopupui.h
FORMS += ../mainwindow.ui ../stationpopupui.ui
RESOURCES += ../imageResource.qrc
msvc: QMAKE_CXXFLAGS += /utf-8
