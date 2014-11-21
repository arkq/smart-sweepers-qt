# smart-sweepers.pro - smart-sweepers-qt
# Copyright (c) 2014 Arkadiusz Bokowy

TEMPLATE = app

TARGET = smart-sweepers-qt
VERSION = 1.0

CONFIG += qt
CONFIG += c++11

QT += core gui
QT += widgets

unix {
	QMAKE_CXXFLAGS += -std=c++11
}

HEADERS += \
	src/MainWindow.h

#	src/C2DMatrix.h \
#	src/CController.h \
#	src/CGenAlg.h \
#	src/CMineSweeper.h \
#	src/CNeuralNet.h \
#	src/CParams.h \
#	src/CTimer.h \
#	src/SVector2D.h \
#	src/MainWindow.h \
#	src/utils.h

SOURCES += \
	src/MainWindow.cpp \
	src/main.cpp

#	src/C2DMatrix.cpp \
#	src/CController.cpp \
#	src/CGenAlg.cpp \
#	src/CMinesweeper.cpp \
#	src/CNeuralNet.cpp \
#	src/CParams.cpp \
#	src/CTimer.cpp \
#	src/MainWindow.cpp \
#	src/main.cpp \
#	src/utils.cpp

FORMS += \
	ui/MainWindow.ui \
	ui/PreferencesDialog.ui
