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
		if(inputPoints.size()<3)return;
		outputPoints.clear(), outputLines.clear();

		bool right, left;
		int valueOfCrossProduct;

		for(unsigned i = 0; i < inputPoints.size(); i ++)
		{
			for(unsigned j = i+1; j < inputPoints.size(); j ++)
			{
				right = left = false;
				for(unsigned k = 0; k < inputPoints.size(); k ++)
				{
					if(i==k||j==k)continue;
					valueOfCrossProduct = (int)Utilities::crossProduct(inputPoints[k], inputPoints[i], inputPoints[j]);
					if(valueOfCrossProduct<0)		left=true;
					else if(valueOfCrossProduct>0)	right=true;
					else	continue; //Colinear

					if(left&right)
						break;
				}
				if(left^right)
					outputLines.push_back(Line(inputPoints[i],inputPoints[j])),
					outputPoints.push_back(inputPoints[i]),
					outputPoints.push_back(inputPoints[j]);
			}
		}
	}
};
#endif