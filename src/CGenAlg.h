// CGenAlg.h
// Copyright (c) 2002 Mat Buckland
//               2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.
//
// Synopsis:
// Genetic algorithm class. This is based for manipulating std::vectors
// of *real* numbers. Used to adjust the weights in a feed-forward neural
// network.

#ifndef CGENALG_H
#define CGENALG_H

#include <vector>

using std::vector;


// create a structure to hold each genome
struct SGenome {

	SGenome() :
			dFitness(0) {  }
	SGenome(vector<double> w, double f) :
			vecWeights(w),
			dFitness(f) {  }

	// overload '<' used for sorting
	friend bool operator<(const SGenome& lhs, const SGenome& rhs) {
		return (lhs.dFitness < rhs.dFitness);
	}

	vector<double> vecWeights;
	double dFitness;

};


// the genetic algorithm class
class CGenAlg {

public:

	CGenAlg(int popsize, int numweights);

	// this runs the GA for one generation
	vector<SGenome> Epoch(vector<SGenome> &old_pop);

	vector<SGenome> GetChromos() const { return m_vecPop; }
	double AverageFitness() const { return m_dTotalFitness / m_iPopSize; }
	double BestFitness() const { return m_dBestFitness; }

private:

	void Crossover(const vector<double> &mum, const vector<double> &dad,
			vector<double> &baby1, vector<double> &baby2);

	void Mutate(vector<double> &chromo);

	SGenome GetChromoRoulette();

	// use to introduce elitism
	void GrabNBest(int NBest, const int NumCopies, vector<SGenome> &vecPop);

	void CalculateBestWorstAvTot();

	void Reset();

	// this holds the entire population of chromosomes
	vector<SGenome> m_vecPop;

	// size of population
	int m_iPopSize;

	// amount of weights per chromo
	int m_iChromoLength;

	// total fitness of population
	double m_dTotalFitness;

	// best fitness this population
	double m_dBestFitness;

	// average fitness
	double m_dAverageFitness;

	// worst
	double m_dWorstFitness;

	// keeps track of the best genome
	int m_iFittestGenome;

	// generation counter
	int m_cGeneration;

};

#endif
