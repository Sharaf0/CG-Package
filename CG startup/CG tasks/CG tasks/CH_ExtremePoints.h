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
		vector<Point> input = inputPoints;
		sort(input.begin(),input.end());
		input.resize(unique(input.begin(),input.end())-input.begin());

		if(input.size()<3)
		{
			if(input.size()==2)
			{
				outputPoints.push_back(input.front());
				outputPoints.push_back(input.back());
				outputLines.push_back(Line(input.front(), input.back()));
			}
			return;
		}

		for(unsigned i = 0; i < input.size(); i ++)
		{
			bool in = false;
			for(unsigned j = 0; j < input.size(); j ++)
			{
				if(i==j)continue;

				for(unsigned k = 0; k < input.size(); k ++)
				{
					if(i==k||j==k)continue;

					for(unsigned l = 0; l < input.size(); l ++)
					{
						if(l==k || l==j || l==i)continue;

						if(Utilities::PointInTriangle(input[i],input[j],input[k],input[l]))
						{
							in = true;
							goto decide;
						}
					}
				}
			}
decide:
			if(!in)
				outputPoints.push_back(input[i]);
		}
	}
};
#endif