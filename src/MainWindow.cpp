// MainWindow.cpp
// Copyright (c) 2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_PreferencesDialog.h"

#include <QSettings>


MainWindow::MainWindow(QApplication &app, QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow),
		app(&app) {

	ui->setupUi(this);

	connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(startSimulation()));
	connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stopSimulation()));
	connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
	connect(ui->actionQuit, SIGNAL(triggered()), &app, SLOT(quit()));
	connect(ui->actionAboutQt, SIGNAL(triggered()), &app, SLOT(aboutQt()));

	stopSimulation();
	resetSettings();
	loadSettings();

}

MainWindow::~MainWindow() {
	saveSettings();
	delete ui;
}

void MainWindow::startSimulation() {
	ui->actionStart->setVisible(false);
	ui->actionStop->setVisible(true);
}

void MainWindow::stopSimulation() {
	ui->actionStart->setVisible(true);
	ui->actionStop->setVisible(false);
}

void MainWindow::showPreferences() {
	PreferencesDialog dialog(this);
	dialog.exec();
}

void MainWindow::loadSettings() {

	QSettings settings;

	iFramesPerSecond = settings.value("iFramesPerSecond", iFramesPerSecond).toInt();

	iNumSweepers = settings.value("iNumSweepers", iNumSweepers).toInt();
	iNumMines = settings.value("iNumMines", iNumMines).toInt();
	iNumTicks = settings.value("iNumTicks", iNumTicks).toInt();
	dMaxTurnRate = settings.value("dMaxTurnRate", dMaxTurnRate).toDouble();
	dMaxSpeed = settings.value("dMaxSpeed", dMaxSpeed).toDouble();
	dSweeperScale = settings.value("dSweeperScale", dSweeperScale).toDouble();
	dMineScale = settings.value("dMineScale", dMineScale).toDouble();

	settings.beginGroup("NeuralNetwork");
	iNumInputs = settings.value("iNumInputs", iNumInputs).toInt();
	iNumHidden = settings.value("iNumHidden", iNumHidden).toInt();
	iNeuronsPerHiddenLayer = settings.value("iNeuronsPerHiddenLayer", iNeuronsPerHiddenLayer).toInt();
	iNumOutputs = settings.value("iNumOutputs", iNumOutputs).toInt();
	dActivationResponse = settings.value("dActivationResponse", dActivationResponse).toDouble();
	dBias = settings.value("dBias", dBias).toDouble();
	settings.endGroup();

	settings.beginGroup("GeneticAlgorithm");
	dCrossoverRate = settings.value("dCrossoverRate", dCrossoverRate).toDouble();
	dMutationRate = settings.value("dMutationRate", dMutationRate).toDouble();
	dMaxPerturbation = settings.value("dMaxPerturbation", dMaxPerturbation).toDouble();
	iNumElite = settings.value("iNumElite", iNumElite).toInt();
	iNumCopiesElite = settings.value("iNumCopiesElite", iNumCopiesElite).toInt();
	settings.endGroup();

}

void MainWindow::saveSettings() {

	QSettings settings;

	settings.setValue("iFramesPerSecond", iFramesPerSecond);

	settings.setValue("iNumSweepers", iNumSweepers);
	settings.setValue("iNumMines", iNumMines);
	settings.setValue("iNumTicks", iNumTicks);
	settings.setValue("dMaxTurnRate", dMaxTurnRate);
	settings.setValue("dMaxSpeed", dMaxSpeed);
	settings.setValue("dSweeperScale", dSweeperScale);
	settings.setValue("dMineScale", dMineScale);

	settings.beginGroup("NeuralNetwork");
	settings.setValue("iNumInputs", iNumInputs);
	settings.setValue("iNumHidden", iNumHidden);
	settings.setValue("iNeuronsPerHiddenLayer", iNeuronsPerHiddenLayer);
	settings.setValue("iNumOutputs", iNumOutputs);
	settings.setValue("dActivationResponse", dActivationResponse);
	settings.setValue("dBias", dBias);
	settings.endGroup();

	settings.beginGroup("GeneticAlgorithm");
	settings.setValue("dCrossoverRate", dCrossoverRate);
	settings.setValue("dMutationRate", dMutationRate);
	settings.setValue("dMaxPerturbation", dMaxPerturbation);
	settings.setValue("iNumElite", iNumElite);
	settings.setValue("iNumCopiesElite", iNumCopiesElite);
	settings.endGroup();

}

void MainWindow::resetSettings() {

	iFramesPerSecond = 60;

	iNumSweepers = 30;
	iNumMines = 40;
	iNumTicks = 2000;
	dMaxTurnRate = 0.3;
	dMaxSpeed = 2;
	dSweeperScale = 5;
	dMineScale = 2;

	iNumInputs = 4;
	iNumHidden = 1;
	iNeuronsPerHiddenLayer = 6;
	iNumOutputs = 2;
	dActivationResponse = 1;
	dBias = -1;

	dCrossoverRate = 0.7;
	dMutationRate = 0.1;
	dMaxPerturbation = 0.3;
	iNumElite = 4;
	iNumCopiesElite = 1;

}


PreferencesDialog::PreferencesDialog(MainWindow *mainwindow) :
		QDialog(mainwindow),
		ui(new Ui::PreferencesDialog),
		mainwindow(mainwindow) {

	ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton *)),
			this, SLOT(buttonAction(QAbstractButton *)));

	loadSettings();

}

PreferencesDialog::~PreferencesDialog() {
	delete ui;
}

void PreferencesDialog::buttonAction(QAbstractButton *button) {
	switch (ui->buttonBox->standardButton(button)) {
	case ui->buttonBox->Ok:
	case ui->buttonBox->Apply:
		applySettings();
		break;
	case ui->buttonBox->RestoreDefaults:
		resetSettings();
		break;
	default:
		break;
	}
}

void PreferencesDialog::applySettings() {

	mainwindow->iFramesPerSecond = ui->framesPerSecond->value();

	mainwindow->iNumSweepers = ui->numSweepers->value();
	mainwindow->iNumMines = ui->numMines->value();
	mainwindow->iNumTicks = ui->numTicks->value();
	mainwindow->dMaxTurnRate = ui->maxTurnRate->value();
	mainwindow->dMaxSpeed = ui->maxSpeed->value();
	mainwindow->dSweeperScale = ui->sweeperScale->value();
	mainwindow->dMineScale = ui->mineScale->value();

	mainwindow->iNumInputs = ui->numInputs->value();
	mainwindow->iNumHidden = ui->numHidden->value();
	mainwindow->iNeuronsPerHiddenLayer = ui->neuronsPerHiddenLayer->value();
	mainwindow->iNumOutputs = ui->numOutputs->value();
	mainwindow->dActivationResponse = ui->activationResponse->value();
	mainwindow->dBias = ui->bias->value();

	mainwindow->dCrossoverRate = ui->crossoverRate->value();
	mainwindow->dMutationRate = ui->mutationRate->value();
	mainwindow->dMaxPerturbation = ui->maxPerturbation->value();
	mainwindow->iNumElite = ui->numElite->value();
	mainwindow->iNumCopiesElite = ui->numCopiesElite->value();

}

void PreferencesDialog::resetSettings() {
	mainwindow->resetSettings();
	loadSettings();
}

void PreferencesDialog::loadSettings() {

	ui->framesPerSecond->setValue(mainwindow->iFramesPerSecond);

	ui->numSweepers->setValue(mainwindow->iNumSweepers);
	ui->numMines->setValue(mainwindow->iNumMines);
	ui->numTicks->setValue(mainwindow->iNumTicks);
	ui->maxTurnRate->setValue(mainwindow->dMaxTurnRate);
	ui->maxSpeed->setValue(mainwindow->dMaxSpeed);
	ui->sweeperScale->setValue(mainwindow->dSweeperScale);
	ui->mineScale->setValue(mainwindow->dMineScale);

	ui->numInputs->setValue(mainwindow->iNumInputs);
	ui->numHidden->setValue(mainwindow->iNumHidden);
	ui->neuronsPerHiddenLayer->setValue(mainwindow->iNeuronsPerHiddenLayer);
	ui->numOutputs->setValue(mainwindow->iNumOutputs);
	ui->activationResponse->setValue(mainwindow->dActivationResponse);
	ui->bias->setValue(mainwindow->dBias);

	ui->crossoverRate->setValue(mainwindow->dCrossoverRate);
	ui->mutationRate->setValue(mainwindow->dMutationRate);
	ui->maxPerturbation->setValue(mainwindow->dMaxPerturbation);
	ui->numElite->setValue(mainwindow->iNumElite);
	ui->numCopiesElite->setValue(mainwindow->iNumCopiesElite);

}
