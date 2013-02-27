#pragma once
#ifndef ALGORITHM
#define ALGORITHM

#include "Utilities.h"

using namespace std;

class Algorithm
{
public:
	//Algorithm(){}
	virtual void run(	const vector<Point>& inputPoints, const vector<Line>& inputLines,
						vector<Point>& outputPoints, vector<Line>& outputLines) = 0;
};
#endif