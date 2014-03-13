#pragma once
#ifndef CH_EXTREMESEGMENTS
#define CH_EXTREMESEGMENTS
#include "Algorithm.h"

class CH_ExtremeSegments : public Algorithm
{
public:
	CH_ExtremeSegments(){}
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

		bool right, left;
		int valueOfCrossProduct;

		for(unsigned i = 0; i < input.size(); i ++)
		{
			for(unsigned j = i+1; j < input.size(); j ++)
			{
				right = left = false;
				for(unsigned k = 0; k < input.size(); k ++)
				{
					if(i==k||j==k)continue;
					//valueOfCrossProduct = (int)Utilities::crossProduct(input[k], input[i], input[j]);
					valueOfCrossProduct = (int)Utilities::cross2Vectors(input[j] - input[i], input[k] - input[i]);
					if(valueOfCrossProduct>0)		left=true;
					else if(valueOfCrossProduct<0)	right=true;
					else
					{
						if(!Utilities::pointOnSegment(input[i], input[j], input[k]))
						{
							left=true;
							right=true;
						}
					}

					if(left&right)
						break;
				}
				if(left^right)
					outputLines.push_back(Line(input[i], input[j])),
					outputPoints.push_back(input[i]),
					outputPoints.push_back(input[j]);
			}
		}
	}
};
#endif