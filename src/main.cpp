// main.cpp
// Copyright (c) 2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.

#include <QApplication>

#include "MainWindow.h"


int main(int argc, char *argv[]) {

	QApplication app(argc, argv);

	// set values required by the settings manager
	app.setOrganizationName("ArkqSoft");
	app.setApplicationName("SmartSweepers");

	MainWindow window(app);
	window.show();

	return app.exec();
}
