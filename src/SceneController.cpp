// SceneController.cpp
// Copyright (c) 2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.

#define _USE_MATH_DEFINES
#include "SceneController.h"

#include "MainWindow.h"
#include "utils.h"

#ifndef M_PI
// if you want something done, do it yourself...
#define M_PI (4 * atan(1))
#endif


// graphic object templates centered at (0, 0)
const QPointF objectMinePoints[] = {
	QPointF(-1, -1), QPointF(1, -1), QPointF(1, 1), QPointF(-1, 1),
};
const QPointF objectSweeperPoints[] = {
	QPointF(0.4, -0.5), QPointF(0.4, -1), QPointF(1, -1),
	QPointF(1, 1), QPointF(0.4, 1), QPointF(0.4, -0.1),
	QPointF(0.2, -0.1), QPointF(0.2, 0.8), QPointF(0.1, 0.8), QPointF(0.1, 1.7),
	QPointF(-0.1, 1.7), QPointF(-0.1, 0.8), QPointF(-0.2, 0.8), QPointF(-0.2, -0.1),
	QPointF(-0.4, -0.1), QPointF(-0.4, 1), QPointF(-1, 1),
	QPointF(-1, -1), QPointF(-0.4, -1), QPointF(-0.4, -0.5),
};


// Initialize the sweepers, their brains and the GA factory.
SceneController::SceneController(int width, int height, QObject *parent) :
		QObject(parent),
		gs(new QGraphicsScene(0, 0, width, height)),
		gsInfo(new QGraphicsSimpleTextItem),
		gsDefaultPen(),
		gsElitePen(Qt::red),
		gsMinePen(Qt::green),
		vpWidth(width),
		vpHeight(height),
		m_pGA(nullptr),
		m_bInternalError(false),
		m_iTicks(0),
		m_iGenerations(0) {

	// MSVC does not support the std::initializer_list, so this is the only way
	// to create our object templates and be platform independent...
	for (unsigned int i = 0; i < sizeof(objectMinePoints) / sizeof(*objectMinePoints); ++i)
		objectMine.append(objectMinePoints[i]);
	for (unsigned int i = 0; i < sizeof(objectSweeperPoints) / sizeof(*objectSweeperPoints); ++i)
		objectSweeper.append(objectSweeperPoints[i]);

	// compatibility mode with Qt5
	gsDefaultPen.setCosmetic(true);
	gsElitePen.setCosmetic(true);
	gsMinePen.setCosmetic(true);

	gs->addItem(gsInfo);

	// let's create the minesweepers
	for (int i = MainWindow::s.iNumSweepers; i; --i)
		vecSweepers.push_back(CMinesweeper(RandFloat() * vpWidth,
					RandFloat() * vpHeight, RandFloat() * 2 * M_PI));

	// and initial population of mines
	updateMineObjects(MainWindow::s.iNumMines);

	// get the total number of weights used in the sweepers
	// NN so we can initialize the GA
	int m_NumWeightsInNN = vecSweepers[0].GetNumberOfWeights();

	// initialize the Genetic Algorithm class
	delete m_pGA;
	m_pGA = new CGenAlg(vecSweepers.size(), m_NumWeightsInNN);

	// get the weights from the GA and insert into the sweepers brains
	vecThePopulation = QVector<SGenome>::fromStdVector(m_pGA->GetChromos());

	for (int i = 0; i < vecThePopulation.size(); i++)
		vecSweepers[i].PutWeights(vecThePopulation[i].vecWeights);

}

SceneController::~SceneController() {
	delete m_pGA;
	delete gsInfo;
	delete gs;
}

void SceneController::setViewport(int width, int height) {
	vpWidth = width;
	vpHeight = height;
	gs->setSceneRect(0, 0, width, height);
}

// Synchronize the number of mine objects with the given value.
void SceneController::updateMineObjects(int number) {

	int diff = number - vecMines.size();

	if (diff > 0) {
		// initialize mines in random positions within the viewport
		for (int i = diff; i; --i)
			vecMines.push_back(SVector2D(RandFloat() * vpWidth, RandFloat() * vpHeight));
	}
	else if (diff < 0)
		vecMines.resize(number);

	// wrap mines around the current viewport
	for (auto i = vecMines.begin(); i < vecMines.end(); ++i) {
		i->x = fmod(i->x, vpWidth);
		i->y = fmod(i->y, vpHeight);
	}

}

// Update our rendering scene. Note, that displaying is separated form
// the actual simulation process.
void SceneController::updateScene() {

	if (m_bInternalError)
		// something goes terribly wrong
		return;

	{ // synchronize scene mines objects with back-end ones
		int diff = vecMines.size() - gsMines.size();
		if (diff > 0) {
			for (int i = diff; i; --i)
				gsMines.push_back(gs->addPolygon(objectMine));
		}
		else if (diff < 0) {
			for (int i = diff; i; ++i) {
				delete gsMines.back();
				gsMines.pop_back();
			}
		}
	}

	{ // synchronize scene minesweeper objects with back-end ones
		int diff = vecSweepers.size() - gsSweepers.size();
		if (diff > 0) {
			for (int i = diff; i; --i)
				gsSweepers.push_back(gs->addPolygon(objectSweeper));
		}
		else if (diff < 0) {
		}
	}

	// update mine positions
	for (auto i = vecMines.size(); i; --i) {
		auto object = vecMines[i - 1];
		auto *item = gsMines[i - 1];
		item->setPos(object.x, object.y);
		item->setScale(MainWindow::s.dMineScale);
		item->setPen(gsMinePen);
	}

	// get the fitness score list and sort it in the descending order
	// in order to get the fitness threshold value for elite classification
	QVector<int> fitness;
	for (auto i = vecSweepers.size(); i; --i)
		fitness.push_back(vecSweepers[i - 1].Fitness());
	qSort(fitness.begin(), fitness.end(), qGreater<int>());
	int fitnessThreshold = fitness[MainWindow::s.iNumElite] + 1;

	// update minesweeper positions, rotations and colors
	for (auto i = vecSweepers.size(); i; --i) {
		auto object = vecSweepers[i - 1];
		auto *item = gsSweepers[i - 1];
		item->setPos(object.Position().x, object.Position().y);
		item->setRotation(object.Rotation() * 180 / M_PI);
		item->setScale(MainWindow::s.dSweeperScale);

		// we want the fittest displayed in a different color
		if (object.Fitness() >= fitnessThreshold)
			item->setPen(gsElitePen);
		else
			item->setPen(gsDefaultPen);
	}

	// update info statistics
	QString textGeneration = QString("Generation: %1 [TTL: %2]\n")
		.arg(m_iGenerations).arg(MainWindow::s.iNumTicks - m_iTicks);
	QString textFitness = QString("Fitness: best: %1, avge: %2\n")
		.arg(m_pGA->BestFitness()).arg(m_pGA->AverageFitness());
	QString textElite = QString("Elite threshold: %1\n").arg(fitnessThreshold);
	gsInfo->setText(textGeneration + textFitness + textElite);

}

// This is the main workhorse. The entire simulation is controlled from here.
// The comments should explain what is going on adequately.
void SceneController::updateSimulation() {

	// NN uses STL (it's not ported into the QTL)
	auto mines = vecMines.toStdVector();

	// Run the sweepers through iNumTicks amount of cycles. During this loop
	// each sweepers NN is constantly updated with the appropriate information
	// from its surroundings. The output from the NN is obtained and the sweeper
	// is moved. If it encounters a mine its fitness is updated appropriately.
	if (m_iTicks++ < MainWindow::s.iNumTicks) {
		for (int i = 0; i < vecSweepers.size(); ++i) {

			// update the NN and position
			if (!vecSweepers[i].Update(mines)) {
				// error in processing the neural net
				gsInfo->setText("ERROR: Wrong amount of NN inputs!");
				m_bInternalError = true;
				return;
			}

			// keep minesweepers in our viewport
			vecSweepers[i].WarpWorld(0, 0, vpWidth, vpHeight);

			// see if it's found a mine
			int GrabHit = vecSweepers[i].CheckForMine(mines, MainWindow::s.dMineScale);
			if (GrabHit >= 0) {
				// we have discovered a mine so increase fitness
				vecSweepers[i].IncrementFitness();
				// mine found so replace the mine with another at a random position
				vecMines[GrabHit] = SVector2D(RandFloat() * vpWidth, RandFloat() * vpHeight);
			}

			// update the chromos fitness score
			vecThePopulation[i].dFitness = vecSweepers[i].Fitness();

		}
	}
	// Another generation has been completed.
	// Time to run the GA and update the sweepers with their new NNs
	else {

		// run the GA to create a new population
		// NN uses STL (it's not ported into the QTL)
		auto population = vecThePopulation.toStdVector();
		vecThePopulation = QVector<SGenome>::fromStdVector(m_pGA->Epoch(population));

		// emit current generation stats
		emit generationStats(m_iGenerations,
				m_pGA->BestFitness(), m_pGA->AverageFitness());

		// insert the new (hopefully) improved brains back into the sweepers
		for (int i = 0; i < vecSweepers.size(); ++i) {
			vecSweepers[i].PutWeights(vecThePopulation[i].vecWeights);
			vecSweepers[i].Respawn();
		}

		// increment the generation counter
		++m_iGenerations;

		// reset cycles
		m_iTicks = 0;

	}

}
