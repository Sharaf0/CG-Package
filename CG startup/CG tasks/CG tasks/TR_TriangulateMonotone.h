#pragma once
#ifndef MONOTONE_TRIANGULATION
#define MONOTONE_TRIANGULATION
#include "Algorithm.h"
class TR_MonotoneTriangulation : public Algorithm
{
public:
	TR_MonotoneTriangulation(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{}
};
#endif