// CNeuralNet.h
// Copyright (c) 2002 Mat Buckland
//               2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.
//
// Synopsis:
// Class for creating a feed-forward neural network.

#ifndef CNEURALNET_H
#define CNEURALNET_H

#include <vector>

using std::vector;


// neuron structure
struct SNeuron {

	SNeuron(int NumInputs);

	// the number of inputs into the neuron
	int m_NumInputs;

	// the weights for each input
	vector<double> m_vecWeight;

};


// structure to hold a layer of neurons
struct SNeuronLayer {

	SNeuronLayer(int NumNeurons, int NumInputsPerNeuron);

	// the number of neurons in this layer
	int m_NumNeurons;

	// the layer of neurons
	vector<SNeuron> m_vecNeurons;

};


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

	// storage for each layer of neurons including the output layer
	vector<SNeuronLayer> m_vecLayers;

};

#endif
