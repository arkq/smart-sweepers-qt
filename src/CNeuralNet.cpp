// CNeuralNet.cpp
// Copyright (c) 2002 Mat Buckland
//               2014-2017 Arkadiusz Bokowy
//
// This file is a part of smart-sweepers-qt.
//
// This project is licensed under the terms of the MIT license.

#include "CNeuralNet.h"

#include <cmath>

#include "MainWindow.h"
#include "utils.h"


// Create a layer of neurons of the required size.
static vector<vector<double>> createNeuronLayer(int numNeurons, int numInputsPerNeuron) {
	vector<vector<double>> layer(numNeurons);
	for (unsigned int i = 0; i < layer.size(); ++i) {
		// we need an additional weight for the bias hence the +1
		vector<double> neuron(numInputsPerNeuron + 1);
		for (unsigned int j = 0; j < neuron.size(); ++j)
			// set up the weights with an initial random value
			neuron[j] = RandomClamped();
		layer[i] = neuron;
	}
	return layer;
}

// Create an Artificial Neural Net.
CNeuralNet::CNeuralNet() :
		m_NumInputs(MainWindow::s.iNumInputs),
		m_NumOutputs(MainWindow::s.iNumOutputs),
		m_NumHiddenLayers(MainWindow::s.iNumHiddenLayers),
		m_NeuronsPerHiddenLyr(MainWindow::s.iNeuronsPerHiddenLayer) {

	// the weights are all initially set to random values -1 < w < 1

	// create the layers of the network
	if (m_NumHiddenLayers > 0) {

		// create first hidden layer
		m_vecLayers.push_back(createNeuronLayer(m_NeuronsPerHiddenLyr, m_NumInputs));

		for (int i = 0; i < m_NumHiddenLayers - 1; ++i)
			m_vecLayers.push_back(createNeuronLayer(m_NeuronsPerHiddenLyr, m_NeuronsPerHiddenLyr));

		// create output layer
		m_vecLayers.push_back(createNeuronLayer(m_NumOutputs, m_NeuronsPerHiddenLyr));

	}
	else {
		// create output layer
		m_vecLayers.push_back(createNeuronLayer(m_NumOutputs, m_NumInputs));
	}

}


// Return a vector containing the weights.
vector<double> CNeuralNet::GetWeights() const {

	vector<double> weights;

	// for each layer
	for (int i = 0; i < m_NumHiddenLayers + 1; ++i)
		// for each neuron
		for (unsigned int j = 0; j < m_vecLayers[i].size(); ++j)
			// for each weight
			for (unsigned int k = 0; k < m_vecLayers[i][j].size(); ++k)
				weights.push_back(m_vecLayers[i][j][k]);

	return weights;
}

// Given a vector of doubles this function replaces the weights in the NN
// with the new values.
void CNeuralNet::PutWeights(vector<double> &weights) {

	int cWeight = 0;

	// for each layer
	for (int i = 0; i < m_NumHiddenLayers + 1; ++i)
		// for each neuron
		for (unsigned int j = 0; j < m_vecLayers[i].size(); ++j)
			// for each weight
			for (unsigned int k = 0; k < m_vecLayers[i][j].size(); ++k)
				m_vecLayers[i][j][k] = weights[cWeight++];

}

// Return the total number of weights needed for the net.
int CNeuralNet::GetNumberOfWeights() const {

	int weights = 0;

	// for each layer
	for (int i = 0; i < m_NumHiddenLayers + 1; ++i)
		// for each neuron
		for (unsigned int j = 0; j < m_vecLayers[i].size(); ++j)
			// for each weight
			for (unsigned int k = 0; k < m_vecLayers[i][j].size(); ++k)
				weights++;

	return weights;
}

// Given an input vector this function calculates the output vector.
vector<double> CNeuralNet::Update(vector<double> &inputs) {

	vector<double> outputs;

	int cWeight = 0;

	// first check that we have the correct amount of inputs
	if (inputs.size() != (unsigned)m_NumInputs)
		// just return an empty vector if incorrect
		return outputs;

	// for each layer....
	for (int i=0; i < m_NumHiddenLayers + 1; ++i) {

		if ( i > 0 )
			inputs = outputs;

		outputs.clear();

		cWeight = 0;

		// For each neuron sum the (inputs * corresponding weights). Throw
		// the total at our sigmoid function to get the output.
		for (unsigned int j = 0; j < m_vecLayers[i].size(); ++j) {

			auto numInputs = m_vecLayers[i][j].size();
			double netinput = 0;

			// for each weight
			for (unsigned int k = 0; k < numInputs - 1; ++k)
				// sum the weights x inputs
				netinput += m_vecLayers[i][j][k] * inputs[cWeight++];

			// add in the bias
			netinput += m_vecLayers[i][j].back() * MainWindow::s.dBias;

			// We can store the outputs from each layer as we generate them.
			// The combined activation is first filtered through the sigmoid
			// function.
			outputs.push_back(Sigmoid(netinput, MainWindow::s.dActivationResponse));

			cWeight = 0;
		}
	}

	return outputs;
}

// sigmoid function
double CNeuralNet::Sigmoid(double netinput, double response) {
	return ( 1 / ( 1 + exp(-netinput / response)));
}
