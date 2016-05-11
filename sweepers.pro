# sweepers.pro - smart-sweepers-qt
# Copyright (c) 2014-2016 Arkadiusz Bokowy

TEMPLATE = app

TARGET = smart-sweepers-qt
VERSION = 1.0

CONFIG += qt
CONFIG += c++11

QT += core gui
QT += widgets

unix:!greaterThan(QT_MAJOR_VERSION, 4) {
	QMAKE_CXXFLAGS += -std=c++11
}

HEADERS += \
	src/CGenAlg.h \
	src/CMinesweeper.h \
	src/CNeuralNet.h \
	src/MainWindow.h \
	src/SceneController.h \
	src/SVector2D.h \
	src/utils.h

SOURCES += \
	src/CGenAlg.cpp \
	src/CMinesweeper.cpp \
	src/CNeuralNet.cpp \
	src/MainWindow.cpp \
	src/SceneController.cpp \
	src/main.cpp

FORMS += \
	src/MainWindow.ui \
	src/PreferencesDialog.ui \
	src/StatisticsDialog.ui
