#pragma once
#ifndef SL_BENTLEYOTTMAN
#define SL_BENTLEYOTTMAN
#include "Algorithm.h"

class SL_BentleyOttman : public Algorithm
{
public:
	SL_BentleyOttman(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
	}
};
#endif