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
#include <QMainWindow>


namespace Ui {
	class MainWindow;
	class PreferencesDialog;
}


class MainWindow : public QMainWindow {
	Q_OBJECT

	// allow settings access
	friend class PreferencesDialog;

public:
	explicit MainWindow(QApplication &app, QWidget *parent = 0);
	~MainWindow();

public slots:
	virtual void startSimulation();
	virtual void stopSimulation();
	virtual void showPreferences();

	virtual void loadSettings();
	virtual void saveSettings();
	virtual void resetSettings();

private:
	Ui::MainWindow *ui;
	QApplication *app;

	////
	// Smart Sweepers Settings

	int iFramesPerSecond;

	// --- used for the neural network ---

	int iNumInputs;
	int iNumHidden;
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

	double dCrossoverRate;
	double dMutationRate;

	// the maximum amount the GA may mutate each weight by
	double dMaxPerturbation;

	// used for elitism
	int iNumElite;
	int iNumCopiesElite;

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

#endif  // MAINWINDOW_H
