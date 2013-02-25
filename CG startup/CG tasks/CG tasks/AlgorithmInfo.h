#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "Point.h"
#include "Line.h"

using namespace std;

class AlgorithmInfo
{
protected:
	AlgorithmInfo()
	{}
public:
	string algorithmName;
	
	virtual void run(	vector<Point> inputPoints, vector<Line>& inputLines,
				vector<Point>& outputPoints, vector<Line>& outputLines) = 0;
};