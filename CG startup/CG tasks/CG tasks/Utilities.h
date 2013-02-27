#pragma once
#ifndef UTILITIES
#define UTILITIES

#include "Line.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;
class Utilities
{
public:
	static float sign( const Point& p1, const Point& p2, const Point& p3)
	{
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	static bool PointInTriangle( const Point& pt, const Point& v1, const Point& v2, const Point& v3)
	{
		bool b1, b2, b3;

		b1 = Utilities::sign(pt, v1, v2) < 0.0f;
		b2 = Utilities::sign(pt, v2, v3) < 0.0f;
		b3 = sign(pt, v3, v1) < 0.0f;

		return ((b1 == b2) && (b2 == b3));
	}
};
#endif