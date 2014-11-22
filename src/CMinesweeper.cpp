// CMinesweeper.cpp
// Copyright (c) 2002 Mat Buckland
//               2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.

#include "CMinesweeper.h"

#include <cmath>

#include "MainWindow.h"
#include "utils.h"


CMinesweeper::CMinesweeper(SVector2D position, double rotation) :
		m_vPosition(position),
		m_dRotation(rotation),
		m_lTrack(0.16),
		m_rTrack(0.16),
		m_iFitness(0),
		m_iClosestMine(0) {

}

// First we take sensor readings and feed these into the sweepers brain.
//
// The inputs are:
// - vector to the closest mine (x, y)
// - the sweepers 'look at' vector (x, y)
//
// We receive two outputs from the brain.. lTrack & rTrack. So given a force
// for each track we calculate the resultant rotation and acceleration and
// apply to current velocity vector.
bool CMinesweeper::Update(vector<SVector2D> &mines) {

	// this will store all the inputs for the NN
	vector<double> inputs;

	// get vector to closest mine
	SVector2D vClosestMine = GetClosestMine(mines);

	// normalize it
	Vec2DNormalize(vClosestMine);

	// add in vector to closest mine
	inputs.push_back(vClosestMine.x);
	inputs.push_back(vClosestMine.y);

	// add in sweepers look at vector
	inputs.push_back(m_vLookAt.x);
	inputs.push_back(m_vLookAt.y);

	// update the brain and get feedback
	vector<double> output = m_ItsBrain.Update(inputs);

	// make sure there were no errors in calculating the output
	if (output.size() < (unsigned)MainWindow::s.iNumOutputs)
		return false;

	// assign the outputs to the sweepers left & right tracks
	m_lTrack = output[0];
	m_rTrack = output[1];

	// calculate steering forces
	double RotForce = m_lTrack - m_rTrack;

	// clamp rotation
	Clamp(RotForce, -MainWindow::s.dMaxTurnRate, MainWindow::s.dMaxTurnRate);

	m_dRotation += RotForce;

	m_dSpeed = (m_lTrack + m_rTrack);

	// update Look At
	m_vLookAt.x = -sin(m_dRotation);
	m_vLookAt.y = cos(m_dRotation);

	// update position
	m_vPosition += m_vLookAt * m_dSpeed;

	return true;
}

// Returns the vector from the sweeper to the closest mine.
SVector2D CMinesweeper::GetClosestMine(vector<SVector2D> &mines) {

	double closest_so_far = 99999;
	SVector2D vClosestObject(0, 0);

	// cycle through mines to find closest
	for (unsigned int i = 0; i < mines.size(); i++) {
		double len_to_object = Vec2DLength(mines[i] - m_vPosition);

		if (len_to_object < closest_so_far) {
			closest_so_far	= len_to_object;
			vClosestObject	= m_vPosition - mines[i];
			m_iClosestMine = i;
		}
	}

	return vClosestObject;
}

// This function checks for collision with its closest mine (calculated
// earlier and stored in m_iClosestMine).
int CMinesweeper::CheckForMine(vector<SVector2D> &mines, double size) {
	SVector2D DistToObject = m_vPosition - mines[m_iClosestMine];
	if (Vec2DLength(DistToObject) < size + 5)
		return m_iClosestMine;
	return -1;
}

// Wrap around given limits.
void CMinesweeper::WarpWorld(int x, int y, int width, int height) {
	if (m_vPosition.x > width)
		m_vPosition.x = x;
	if (m_vPosition.x < x)
		m_vPosition.x = width;
	if (m_vPosition.y > height)
		m_vPosition.y = y;
	if (m_vPosition.y < y)
		m_vPosition.y = height;
}
