#pragma once

#include <math.h>
#include <string>
#include <limits>
#include <cassert>
#include <Box2D\Common\b2Math.h>

#define RADIANS_TO_DEGREES(ANGLE) ((ANGLE) / b2_pi * 180.0)

#define METERS_TO_PIXELS 25


inline double DistanceSq(const b2Vec2 &v1, const b2Vec2 &v2)
{
	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

//returns a random double between zero and 1
inline double RandFloat() {return ((rand())/(RAND_MAX+1.0));}

//returns a random integer between x and y
inline int RandInt(int x,int y)
{
	assert(y>=x && "<RandInt>: y is less than x");
	return rand()%(y-x+1)+x;
}

inline double RandInRange(double x, double y)
{
	return x + RandFloat()*(y-x);
}

inline double Dot(const b2Vec2 & v1, const b2Vec2 & v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

inline b2Vec2 Divide(const b2Vec2 &lhs, double val)
{
	b2Vec2 result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}

inline void Truncate(b2Vec2& vector, double max)
{
	if (vector.Length() > max)
	{
		vector.Normalize();

		vector *= max;
	} 
}



