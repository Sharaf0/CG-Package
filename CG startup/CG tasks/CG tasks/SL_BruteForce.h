#pragma once
#ifndef SL_BRUTEFORCE
#define SL_BRUTEFORCE
#include "Algorithm.h"

class SL_BruteForce : public Algorithm
{
public:
	SL_BruteForce(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		outputPoints.clear(), outputLines.clear();
		for(unsigned i = 0; i < inputLines.size(); i ++)
		{
			for(unsigned j = i+1; j < inputLines.size(); j ++)
			{
				if(/**/1)
					;
			}
		}
	}
};
#endif