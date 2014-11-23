Smart Sweepers Qt
=================

This application is an Qt port of Smart Sweepers v1.0 (see
[Acknowledgment](#acknowledgment)) with some minor changes.

Smart Sweepers is a simple (educational) simulation framework which uses
the Artificial Neural Network (ANN) as an engine for a simple AI model.
Used ANN is a feedforward network evaluated with an unsupervised learning.
Learning algorithm is based on the gene expression programming (Genetic
Algorithm).

In this program, the ANN is used to control the movement of a minesweeper by
changing the speed of a left and a right track (speed difference generates
rotation force). As an input we've got the position of the closest mine and the
minesweeper current direction vector. Genetic Algorithm is used to maximize
the fitness factor, which in our example is the best approximation of a linear
motion toward the closest mine.

![Screenshot](/../screenshots/screenshots/smart-sweepers-qt-win8.png?raw=true)


Compilation
-----------

	$ qmake && make


Acknowledgment
--------------
This program was originally written by Mat Buckland, as a part of his
[tutorial](http://www.ai-junkie.com/ann/evolved/nnt1.html) about the Neural
Network architecture and basic application.
