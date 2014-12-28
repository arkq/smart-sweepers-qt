// MainWindow.cpp
// Copyright (c) 2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_PreferencesDialog.h"
#include "ui_StatisticsDialog.h"

#include <QClipboard>
#include <QSettings>

#include "SceneController.h"


// initialize static settings storage
MainWindow::SmartSweepersSettings MainWindow::s;

MainWindow::MainWindow(QApplication &app, QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow),
		app(&app),
		started(false),
		paused(false),
		simulation_timerid(0),
		render_timerid(0),
		controller(nullptr) {

	ui->setupUi(this);

	dlgstats = new StatisticsDialog(this);
	connect(this, SIGNAL(statisticsUpdated()), dlgstats, SLOT(appendNewData()));

	connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(startSimulation()));
	connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stopSimulation()));
	connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(pauseSimulation()));
	connect(ui->actionStatistics, SIGNAL(triggered()), this, SLOT(showStatistics()));
	connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
	connect(ui->actionAboutQt, SIGNAL(triggered()), &app, SLOT(aboutQt()));

	stopSimulation();
	resetSettings();
	loadSettings();

}

MainWindow::~MainWindow() {
	saveSettings();
	delete controller;
	delete dlgstats;
	delete ui;
}

void MainWindow::startSimulation() {
	ui->actionStart->setVisible(false);
	ui->actionStop->setVisible(true);
	ui->actionPause->setEnabled(true);
	started = true;
	paused = false;

	// reinitialize controller (allow NN reconfiguration)

	delete controller;
	statistics.clear();
	dlgstats->clearData();

	QSize size = ui->graphicsView->size();
	controller = new SceneController(size.width(), size.height());
	ui->graphicsView->setScene(controller->scene());

	connect(controller, SIGNAL(generationStats(int, double, double)),
			this, SLOT(updateStats(int, double, double)));

	startSimulationTimer();
	startRenderTimer();

}

void MainWindow::stopSimulation() {
	ui->actionStart->setVisible(true);
	ui->actionStop->setVisible(false);
	ui->actionPause->setEnabled(false);
	ui->actionPause->setChecked(false);
	started = paused = false;
	stopSimulationTimer();
	stopRenderTimer();
}

void MainWindow::pauseSimulation() {
	if (started) {
		paused ^= true;
		if (paused) {
			stopSimulationTimer();
			stopRenderTimer();
		}
		else {
			startSimulationTimer();
			startRenderTimer();
		}
	}
}

void MainWindow::updateTimers() {
	if (simulation_timerid) {
		stopSimulationTimer();
		startSimulationTimer();
	}
	if (render_timerid) {
		stopRenderTimer();
		startRenderTimer();
	}
}

void MainWindow::updateMines() {
	if (controller)
		controller->updateMineObjects(s.iNumMines);
}

void MainWindow::updateStats(int generation, double bestFitness, double avgeFitness) {
	statistics.push_back(StatisticData(generation, bestFitness, avgeFitness));
	emit statisticsUpdated();
}

void MainWindow::showStatistics() {
	dlgstats->show();
	dlgstats->raise();
	dlgstats->activateWindow();
}

void MainWindow::showPreferences() {
	PreferencesDialog dialog(this);
	dialog.exec();
}

void MainWindow::startSimulationTimer() {
	if (!simulation_timerid) {
		if (s.iCyclesPerSecond)
			simulation_timerid = startTimer(1000 / s.iCyclesPerSecond);
		else
			simulation_timerid = startTimer(0);
	}
}

void MainWindow::stopSimulationTimer() {
	if (simulation_timerid) {
		killTimer(simulation_timerid);
		simulation_timerid = 0;
	}
}

void MainWindow::startRenderTimer() {
	if (!render_timerid) {
		if (s.iFramesPerSecond)
			render_timerid = startTimer(1000 / s.iFramesPerSecond);
		else
			// lets hope that -1 is not a real timer ID
			render_timerid = -1;
	}
}

void MainWindow::stopRenderTimer() {
	if (render_timerid) {
		killTimer(render_timerid);
		render_timerid = 0;
	}
}

void MainWindow::closeEvent(QCloseEvent *event) {
	Q_UNUSED(event);
	app->quit();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	Q_UNUSED(event);
	if (controller) {
		QSize size = ui->graphicsView->size();
		controller->setViewport(size.width(), size.height());
	}
}

void MainWindow::timerEvent(QTimerEvent *event) {
	if (event->timerId() == simulation_timerid)
		controller->updateSimulation();
	if (event->timerId() == render_timerid)
		controller->updateScene();
}

void MainWindow::loadSettings() {

	QSettings settings;

	s.iCyclesPerSecond = settings.value("iCyclesPerSecond", s.iCyclesPerSecond).toInt();
	s.iFramesPerSecond = settings.value("iFramesPerSecond", s.iFramesPerSecond).toInt();

	s.iNumSweepers = settings.value("iNumSweepers", s.iNumSweepers).toInt();
	s.iNumMines = settings.value("iNumMines", s.iNumMines).toInt();
	s.iNumTicks = settings.value("iNumTicks", s.iNumTicks).toInt();
	s.dMaxTurnRate = settings.value("dMaxTurnRate", s.dMaxTurnRate).toDouble();
	s.dMaxSpeed = settings.value("dMaxSpeed", s.dMaxSpeed).toDouble();
	s.dSweeperScale = settings.value("dSweeperScale", s.dSweeperScale).toDouble();
	s.dMineScale = settings.value("dMineScale", s.dMineScale).toDouble();

	settings.beginGroup("NeuralNetwork");
	s.iNumInputs = settings.value("iNumInputs", s.iNumInputs).toInt();
	s.iNumHiddenLayers = settings.value("iNumHiddenLayers", s.iNumHiddenLayers).toInt();
	s.iNeuronsPerHiddenLayer = settings.value("iNeuronsPerHiddenLayer", s.iNeuronsPerHiddenLayer).toInt();
	s.iNumOutputs = settings.value("iNumOutputs", s.iNumOutputs).toInt();
	s.dActivationResponse = settings.value("dActivationResponse", s.dActivationResponse).toDouble();
	s.dBias = settings.value("dBias", s.dBias).toDouble();
	settings.endGroup();

	settings.beginGroup("GeneticAlgorithm");
	s.dCrossoverRate = settings.value("dCrossoverRate", s.dCrossoverRate).toDouble();
	s.dMutationRate = settings.value("dMutationRate", s.dMutationRate).toDouble();
	s.dMaxPerturbation = settings.value("dMaxPerturbation", s.dMaxPerturbation).toDouble();
	s.iNumElite = settings.value("iNumElite", s.iNumElite).toInt();
	s.iNumCopiesElite = settings.value("iNumCopiesElite", s.iNumCopiesElite).toInt();
	settings.endGroup();

}

void MainWindow::saveSettings() {

	QSettings settings;

	settings.setValue("iCyclesPerSecond", s.iCyclesPerSecond);
	settings.setValue("iFramesPerSecond", s.iFramesPerSecond);

	settings.setValue("iNumSweepers", s.iNumSweepers);
	settings.setValue("iNumMines", s.iNumMines);
	settings.setValue("iNumTicks", s.iNumTicks);
	settings.setValue("dMaxTurnRate", s.dMaxTurnRate);
	settings.setValue("dMaxSpeed", s.dMaxSpeed);
	settings.setValue("dSweeperScale", s.dSweeperScale);
	settings.setValue("dMineScale", s.dMineScale);

	settings.beginGroup("NeuralNetwork");
	settings.setValue("iNumInputs", s.iNumInputs);
	settings.setValue("iNumHiddenLayers", s.iNumHiddenLayers);
	settings.setValue("iNeuronsPerHiddenLayer", s.iNeuronsPerHiddenLayer);
	settings.setValue("iNumOutputs", s.iNumOutputs);
	settings.setValue("dActivationResponse", s.dActivationResponse);
	settings.setValue("dBias", s.dBias);
	settings.endGroup();

	settings.beginGroup("GeneticAlgorithm");
	settings.setValue("dCrossoverRate", s.dCrossoverRate);
	settings.setValue("dMutationRate", s.dMutationRate);
	settings.setValue("dMaxPerturbation", s.dMaxPerturbation);
	settings.setValue("iNumElite", s.iNumElite);
	settings.setValue("iNumCopiesElite", s.iNumCopiesElite);
	settings.endGroup();

}

void MainWindow::resetSettings() {

	s.iCyclesPerSecond = 60;
	s.iFramesPerSecond = 10;

	s.iNumSweepers = 30;
	s.iNumMines = 40;
	s.iNumTicks = 2000;
	s.dMaxTurnRate = 0.3;
	s.dMaxSpeed = 2;
	s.dSweeperScale = 5;
	s.dMineScale = 2;

	s.iNumInputs = 4;
	s.iNumHiddenLayers = 1;
	s.iNeuronsPerHiddenLayer = 6;
	s.iNumOutputs = 2;
	s.dActivationResponse = 1;
	s.dBias = -1;

	s.dCrossoverRate = 0.7;
	s.dMutationRate = 0.1;
	s.dMaxPerturbation = 0.3;
	s.iNumElite = 4;
	s.iNumCopiesElite = 1;

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
	case ui->buttonBox->RestoreDefaults:
		resetSettings();
	case ui->buttonBox->Ok:
	case ui->buttonBox->Apply:
		applySettings();
	default:
		break;
	}
}

void PreferencesDialog::applySettings() {

	mainwindow->s.iCyclesPerSecond = ui->cyclesPerSecond->value();
	mainwindow->s.iFramesPerSecond = ui->framesPerSecond->value();

	mainwindow->s.iNumSweepers = ui->numSweepers->value();
	mainwindow->s.iNumMines = ui->numMines->value();
	mainwindow->s.iNumTicks = ui->numTicks->value();
	mainwindow->s.dMaxTurnRate = ui->maxTurnRate->value();
	mainwindow->s.dMaxSpeed = ui->maxSpeed->value();
	mainwindow->s.dSweeperScale = ui->sweeperScale->value();
	mainwindow->s.dMineScale = ui->mineScale->value();

	mainwindow->s.iNumInputs = ui->numInputs->value();
	mainwindow->s.iNumHiddenLayers = ui->numHiddenLayers->value();
	mainwindow->s.iNeuronsPerHiddenLayer = ui->neuronsPerHiddenLayer->value();
	mainwindow->s.iNumOutputs = ui->numOutputs->value();
	mainwindow->s.dActivationResponse = ui->activationResponse->value();
	mainwindow->s.dBias = ui->bias->value();

	mainwindow->s.dCrossoverRate = ui->crossoverRate->value();
	mainwindow->s.dMutationRate = ui->mutationRate->value();
	mainwindow->s.dMaxPerturbation = ui->maxPerturbation->value();
	mainwindow->s.iNumElite = ui->numElite->value();
	mainwindow->s.iNumCopiesElite = ui->numCopiesElite->value();

	mainwindow->updateTimers();
	mainwindow->updateMines();

}

void PreferencesDialog::resetSettings() {
	mainwindow->resetSettings();
	loadSettings();
}

void PreferencesDialog::loadSettings() {

	ui->cyclesPerSecond->setValue(mainwindow->s.iCyclesPerSecond);
	ui->framesPerSecond->setValue(mainwindow->s.iFramesPerSecond);

	ui->numSweepers->setValue(mainwindow->s.iNumSweepers);
	ui->numMines->setValue(mainwindow->s.iNumMines);
	ui->numTicks->setValue(mainwindow->s.iNumTicks);
	ui->maxTurnRate->setValue(mainwindow->s.dMaxTurnRate);
	ui->maxSpeed->setValue(mainwindow->s.dMaxSpeed);
	ui->sweeperScale->setValue(mainwindow->s.dSweeperScale);
	ui->mineScale->setValue(mainwindow->s.dMineScale);

	ui->numInputs->setValue(mainwindow->s.iNumInputs);
	ui->numHiddenLayers->setValue(mainwindow->s.iNumHiddenLayers);
	ui->neuronsPerHiddenLayer->setValue(mainwindow->s.iNeuronsPerHiddenLayer);
	ui->numOutputs->setValue(mainwindow->s.iNumOutputs);
	ui->activationResponse->setValue(mainwindow->s.dActivationResponse);
	ui->bias->setValue(mainwindow->s.dBias);

	ui->crossoverRate->setValue(mainwindow->s.dCrossoverRate);
	ui->mutationRate->setValue(mainwindow->s.dMutationRate);
	ui->maxPerturbation->setValue(mainwindow->s.dMaxPerturbation);
	ui->numElite->setValue(mainwindow->s.iNumElite);
	ui->numCopiesElite->setValue(mainwindow->s.iNumCopiesElite);

}


StatisticsDialog::StatisticsDialog(MainWindow *mainwindow) :
		ui(new Ui::StatisticsDialog),
		mainwindow(mainwindow) {

	ui->setupUi(this);

}

StatisticsDialog::~StatisticsDialog() {
	delete ui;
}

void StatisticsDialog::appendNewData() {
	appendData(mainwindow->getStatistics().back());
}

void StatisticsDialog::clearData() {
	ui->tableWidget->setRowCount(0);
}

void StatisticsDialog::appendData(const StatisticData &data) {

	int row = ui->tableWidget->rowCount();
	ui->tableWidget->setRowCount(row + 1);

	ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(data.generation)));
	ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(data.bestFitness)));
	ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(data.avgeFitness)));

}

void StatisticsDialog::keyPressEvent(QKeyEvent *event) {
	if (event->matches(QKeySequence::Copy)) {

		QModelIndexList cells = ui->tableWidget->selectionModel()->selectedIndexes();
		qSort(cells); // sort cells by (row, column)

		QString text;
		int currentRow = 0;
		for (auto i = cells.begin(); i < cells.end(); ++i) {
			if (!text.isEmpty()) {
				if (currentRow == (*i).row())
					text += '\t';
				else
					text += '\n';
			}
			currentRow = (*i).row();
			text += (*i).data().toString();
		}

		mainwindow->getApplication()->clipboard()->setText(text);
	}
}
