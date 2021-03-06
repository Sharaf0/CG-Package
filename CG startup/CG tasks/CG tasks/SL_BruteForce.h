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
		Line l1, l2;
		Point intersection;
		for(unsigned i = 0; i < inputLines.size(); i ++)
		{
			l1 = inputLines[i];
			for(unsigned j = i+1; j < inputLines.size(); j ++)
			{
				l2 = inputLines[j];
				if(Utilities::computeSegmentIntersection(l1.start,l1.end,l2.start,l2.end,intersection))
					outputPoints.push_back(intersection);
			}
		}
	}
};
#endif