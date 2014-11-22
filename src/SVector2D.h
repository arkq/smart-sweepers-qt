// SVector2D.h
// Copyright (c) 2002 Mat Buckland
//               2014 Arkadiusz Bokowy
//
// This file is a part of an smart-sweepers-qt.
//
// This projected is licensed under the terms of the MIT license.
//
// Synopsis:
// Point and 2D Vector structure and methods

#ifndef SVECTOR2D_H
#define SVECTOR2D_H

#include <cmath>


struct SPoint {

	SPoint(float x = 0, float y = 0) :
			x(x), y(y) {  }

	float x, y;

};


struct SVector2D {

	SVector2D(double x = 0, double y = 0) :
		x(x), y(y) {  }

	// we need some overloaded operators
	SVector2D &operator+=(const SVector2D &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	SVector2D &operator-=(const SVector2D &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	SVector2D &operator*=(const double &rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}

	SVector2D &operator/=(const double &rhs) {
		x /= rhs;
		y /= rhs;
		return *this;
	}

	double x, y;

};

// overload the * operator
inline SVector2D operator*(const SVector2D &lhs, double rhs) {
	SVector2D result(lhs);
	result *= rhs;
	return result;
}

inline SVector2D operator*(double lhs, const SVector2D &rhs) {
	SVector2D result(rhs);
	result *= lhs;
	return result;
}

// overload the - operator
inline SVector2D operator-(const SVector2D &lhs, const SVector2D &rhs) {
	SVector2D result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;
	return result;
}

// return the length of a 2D vector
inline double Vec2DLength(const SVector2D &v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

// normalize a 2D Vector
inline void Vec2DNormalize(SVector2D &v) {
	double vector_length = Vec2DLength(v);
	v.x = v.x / vector_length;
	v.y = v.y / vector_length;
}

// calculate the dot product
inline double Vec2DDot(SVector2D &v1, SVector2D &v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

// return positive if v2 is clockwise of v1, minus if anticlockwise
inline int Vec2DSign(SVector2D &v1, SVector2D &v2) {
	return v1.y * v2.x > v1.x * v2.y ? 1 : -1;
}

#endif
