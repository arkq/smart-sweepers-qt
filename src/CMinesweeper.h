// CMinesweeper.h
// Copyright (c) 2002 Mat Buckland
//               2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.
//
// Synopsis:
// Class to create a minesweeper object.

#ifndef CMINESWEEPER_H
#define CMINESWEEPER_H

#include <vector>

#include "CNeuralNet.h"
#include "SVector2D.h"

using std::vector;


class CMinesweeper {

public:

	CMinesweeper(SVector2D position, double rotation);
	CMinesweeper(int x = 0, int y = 0, double rotation = 0) :
			CMinesweeper(SVector2D(x, y), rotation) {  }

	// updates the ANN with information from the sweepers environment
	bool Update(vector<SVector2D> &mines);

	// reset rotation and fitness
	void Respawn() { m_dRotation = m_iFitness = 0; };

	// returns a vector to the closest mine
	SVector2D GetClosestMine(vector<SVector2D> &objects);

	// checks to see if the minesweeper has 'collected' a mine
	int CheckForMine(vector<SVector2D> &mines, double size);

	// warp the minesweeper position to fit into given viewport
	void WarpWorld(int x, int y, int width, int height);

	SVector2D Position() const { return m_vPosition; }
	double Rotation() const { return m_dRotation; }

	void IncrementFitness() { ++m_iFitness; }

	int Fitness() const { return m_iFitness; }

	void PutWeights(vector<double> &w) { m_ItsBrain.PutWeights(w); }

	int GetNumberOfWeights() const { return m_ItsBrain.GetNumberOfWeights(); }

private:

	// the minesweeper's neural net
	CNeuralNet m_ItsBrain;

	// its position in the world
	SVector2D m_vPosition;

	// direction sweeper is facing
	SVector2D m_vLookAt;

	// its rotation (surprise surprise)
	double m_dRotation;

	double m_dSpeed;

	// to store output from the ANN
	double m_lTrack, m_rTrack;

	// the sweeper's fitness score
	int m_iFitness;

	// index position of closest mine
	int m_iClosestMine;

};

#endif
