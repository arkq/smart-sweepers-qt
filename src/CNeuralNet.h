// CNeuralNet.h
// Copyright (c) 2002 Mat Buckland
//               2014-2017 Arkadiusz Bokowy
//
// This file is a part of smart-sweepers-qt.
//
// This project is licensed under the terms of the MIT license.
//
// Synopsis:
// Class for creating a feed-forward neural network.

#ifndef SMARTSWEEPERSQT_CNEURALNET_H_
#define SMARTSWEEPERSQT_CNEURALNET_H_

#include <vector>

using std::vector;


// neural net class
class CNeuralNet {

public:

	CNeuralNet();

	// gets the weights from the NN
	vector<double> GetWeights() const;

	// returns total number of weights in net
	int GetNumberOfWeights() const;

	// replaces the weights with new ones
	void PutWeights(vector<double> &weights);

	// calculates the outputs from a set of inputs
	vector<double> Update(vector<double> &inputs);

	// sigmoid response curve
	double Sigmoid(double activation, double response);

private:

	int m_NumInputs;
	int m_NumOutputs;
	int m_NumHiddenLayers;
	int m_NeuronsPerHiddenLyr;

	// Storage for each layer of neurons including the output layer. This
	// vector should be read as follows: layers->neurons->input_weights
	vector<vector<vector<double>>> m_vecLayers;

};

#endif
