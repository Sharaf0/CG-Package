#pragma once
#ifndef CH_EXTREMEPOINTS
#define CH_EXTREMEPOINTS
#include "Algorithm.h"

class CH_ExtremePoints : public Algorithm
{
public:
	CH_ExtremePoints(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		if(inputPoints.size()<3)return;

		outputPoints.clear();
		for(unsigned i = 0; i < inputPoints.size(); i ++)
		{
			bool in = false;
			for(unsigned j = 0; j < inputPoints.size(); j ++)
			{
				if(i==j)continue;

				for(unsigned k = 0; k < inputPoints.size(); k ++)
				{
					if(i==k||j==k)continue;

					for(unsigned l = 0; l < inputPoints.size(); l ++)
					{
						if(l==k || l==j || l==i)continue;

						if(Utilities::PointInTriangle(inputPoints[i],inputPoints[j],inputPoints[k],inputPoints[l]))
						{
							in = true;
							goto decide;
						}
					}
				}
			}
decide:
			if(!in)
				outputPoints.push_back(inputPoints[i]);
		}
	}
};
#endif