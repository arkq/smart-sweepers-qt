// MainWindow.h
// Copyright (c) 2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMainWindow>
#include <QResizeEvent>
#include <QTimerEvent>
#include <QVector>


namespace Ui {
	class MainWindow;
	class PreferencesDialog;
	class StatisticsDialog;
}


// storage structure for our statistics
struct StatisticData {

	// laziness...
	StatisticData(int generation = 0, double bestFitness = 0, double avgeFitness = 0) :
			generation(generation), bestFitness(bestFitness), avgeFitness(avgeFitness) {  }

	int generation;
	double bestFitness;
	double avgeFitness;

};


class SceneController;
class StatisticsDialog;
class MainWindow : public QMainWindow {
	Q_OBJECT

public:

	explicit MainWindow(QApplication &app, QWidget *parent = 0);
	~MainWindow();

	QApplication *getApplication() { return app; }
	const QVector<StatisticData> &getStatistics() { return statistics; }

	// Smart Sweepers Settings
	static struct SmartSweepersSettings {

		int iCyclesPerSecond;
		int iFramesPerSecond;

		// --- used for the neural network ---

		int iNumInputs;
		int iNumHiddenLayers;
		int iNeuronsPerHiddenLayer;
		int iNumOutputs;

		// for tweaking the sigmoid function
		double dActivationResponse;
		// bias value
		double dBias;

		// --- used to define the sweepers ---

		// limits how fast the sweepers can turn
		double dMaxTurnRate;

		double dMaxSpeed;

		// for controlling the size
		double dSweeperScale;
		double dMineScale;

		// --- controller parameters ---

		int iNumSweepers;
		int iNumMines;

		// number of time steps we allow for each generation to live
		int iNumTicks;

		// --- GA parameters ---

		// probability of chromosones crossing over bits
		// 0.7 is pretty good
		double dCrossoverRate;

		// probability that a chromosones bits will mutate
		// try figures around 0.05 to 0.3
		double dMutationRate;

		// the maximum amount the GA may mutate each weight by
		double dMaxPerturbation;

		// used for elitism
		int iNumElite;
		int iNumCopiesElite;

	} s;

public slots:

	virtual void startSimulation();
	virtual void stopSimulation();
	virtual void pauseSimulation();

	virtual void updateTimers();
	virtual void updateMines();
	virtual void updateStats(int generation, double bestFitness, double avgeFitness);

	virtual void showStatistics();
	virtual void showPreferences();

	virtual void loadSettings();
	virtual void saveSettings();
	virtual void resetSettings();

signals:

	// signalize new data in our stats container
	void statisticsUpdated();

protected:

	void startSimulationTimer();
	void stopSimulationTimer();
	void startRenderTimer();
	void stopRenderTimer();

	void closeEvent(QCloseEvent *event);
	void resizeEvent(QResizeEvent *event);
	void timerEvent(QTimerEvent *event);

private:

	Ui::MainWindow *ui;
	QApplication *app;
	StatisticsDialog *dlgstats;

	bool started;
	bool paused;

	int simulation_timerid;
	int render_timerid;

	// sweepers and mines controller handler
	SceneController *controller;

	// container for statistics for current simulation
	QVector<StatisticData> statistics;

};


class PreferencesDialog : public QDialog {
	Q_OBJECT

public:
	explicit PreferencesDialog(MainWindow *mainwindow);
	~PreferencesDialog();

public slots:
	virtual void buttonAction(QAbstractButton *button);
	virtual void applySettings();
	virtual void resetSettings();

protected:
	void loadSettings();

private:
	Ui::PreferencesDialog *ui;
	MainWindow *mainwindow;

};


class StatisticsDialog : public QDialog {
	Q_OBJECT

public:
	explicit StatisticsDialog(MainWindow *mainwindow);
	~StatisticsDialog();

public slots:
	virtual void appendNewData();
	virtual void clearData();

protected:
	void appendData(const StatisticData &data);
	void keyPressEvent(QKeyEvent *event);

private:
	Ui::StatisticsDialog *ui;
	MainWindow *mainwindow;

};

#endif  // MAINWINDOW_H
