#pragma once

#include <vector>
#include "Point.h"
#include "Line.h"
using namespace std;

class Algorithm
{
public:
	//Algorithm(){}
	virtual void run(	vector<Point>& inputPoints, vector<Line>& inputLines,
						vector<Point>& outputPoints, vector<Line>& outputLines) = 0;
};