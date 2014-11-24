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
#include <QPolygonF>
#include <QVector>

#include "CGenAlg.h"
#include "CMinesweeper.h"
#include "SVector2D.h"


class SceneController : public QObject {
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

signals:

	// signal emitted upon current generation life-time end, can be used
	// to collect simulation statistics
	void generationStats(int generation, double bestFitness, double avgeFitness);

private:

	QGraphicsScene *gs;
	QGraphicsSimpleTextItem *gsInfo;
	QVector<QGraphicsPolygonItem *> gsMines;
	QVector<QGraphicsPolygonItem *> gsSweepers;

	// pen used for painting mines and minesweepers
	QPen gsDefaultPen;
	QPen gsElitePen;
	QPen gsMinePen;

	// graphic object templates
	QPolygonF objectMine;
	QPolygonF objectSweeper;

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

	// cycles per generation
	int m_iTicks;

	// generation counter
	int m_iGenerations;

};

#endif
