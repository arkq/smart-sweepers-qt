// utils.h
// Copyright (c) 2002 Mat Buckland
//               2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.

#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>


// returns a random integer between x and y
inline int RandInt(int x, int y) {
	return rand() % (y - x + 1) + x;
}

// returns a random float between zero and 1
inline double RandFloat() {
	return rand() / (RAND_MAX + 1.0);
}

// returns a random float in the range -1 < n < 1
inline double RandomClamped() {
	return RandFloat() - RandFloat();
}

// clamps the first argument between the second two
inline void Clamp(double &arg, double min, double max) {
	if (arg < min)
		arg = min;
	else if (arg > max)
		arg = max;
}

#endif
