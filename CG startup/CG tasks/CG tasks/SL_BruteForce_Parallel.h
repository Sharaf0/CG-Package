#pragma once
#ifndef SL_BRUTEFORCE_PARALLEL
#define SL_BRUTEFORCE_PARALLEL
#include "Algorithm.h"
#include <omp.h>

class SL_BruteForce_Parallel : public Algorithm
{
public:
	SL_BruteForce_Parallel(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		outputPoints.clear(), outputLines.clear();
		Line l1, l2;
		Point intersection;
		
#pragma omp parallel num_threads(20) private(l1, l2, intersection) shared(inputLines,outputLines)
{
#pragma omp for
		for(int i = 0; i < inputLines.size(); i ++)
		{
			l1 = inputLines[i];
			for(int j = i+1; j < inputLines.size(); j ++)
			{
				l2 = inputLines[j];
				if(Utilities::computeSegmentIntersection(l1.start,l1.end,l2.start,l2.end,intersection))
#pragma omp critical
				{
					outputPoints.push_back(intersection);
				}
			}
		}
}
	}
};
#endif
