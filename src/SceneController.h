// SceneController.h
// Copyright (c) 2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.
//
// Synopsis:
// Controller class for the 'Smart Sweepers' scene.

#ifndef SCENECONTROLER_H
#define SCENECONTROLER_H

#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QObject>
#include <QVector>

#include "CGenAlg.h"
#include "CMinesweeper.h"
#include "SVector2D.h"


class SceneController : QObject {
	Q_OBJECT

public:

	SceneController(int width = 0, int height = 0, QObject *parent = 0);
	~SceneController();

	QGraphicsScene *scene() const { return gs; }

public slots:

	virtual void setViewport(int width, int height);
	virtual void updateMineObjects(int number);

	virtual void updateScene();
	virtual void updateSimulation();

private:

	QGraphicsScene *gs;
	QGraphicsSimpleTextItem *gsInfo;
	QVector<QGraphicsPolygonItem *> gsMines;
	QVector<QGraphicsPolygonItem *> gsSweepers;

	// pen used for painting mines and minesweepers
	QPen gsDefaultPen;
	QPen gsElitePen;
	QPen gsMinePen;

	// storage for the population of genomes, minesweepers and mines
	QVector<SGenome> vecThePopulation;
	QVector<CMinesweeper> vecSweepers;
	QVector<SVector2D> vecMines;

	// internal viewport dimensions
	int vpWidth;
	int vpHeight;

	// pointer to the GA
	CGenAlg *m_pGA;

	// indicates NN processing error
	bool m_bInternalError;

	// stores the average fitness per generation for use in graphing
	QVector<double> m_vecAvFitness;

	// stores the best fitness per generation
	QVector<double> m_vecBestFitness;

	// cycles per generation
	int m_iTicks;

	// generation counter
	int m_iGenerations;

};

#endif
