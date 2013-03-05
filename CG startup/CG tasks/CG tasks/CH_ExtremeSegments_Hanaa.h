#pragma once
#ifndef CH_EXTREME_SEGMENTS_HANAA
#define CH_EXTREME_SEGMENTS_HANAA

#include "Algorithm.h"


class CH_ExtremeSegments_Hanaa : public Algorithm
{
private:




public:
	CH_ExtremeSegments_Hanaa(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		if(inputPoints.size()<3)return;
		outputPoints.clear(), outputLines.clear();

		for (int i = 0 ; i < inputPoints.size(); i++)
		{
			for (int j = i+1 ; j < inputPoints.size(); j++)
			{
				bool right = false, left = false;

				for (int k = 0 ; k < inputPoints.size() ; k++)
				{
					if (k!=i && k!=j)
					{
						float Product  = Utilities::crossProduct(inputPoints[k], inputPoints[i], inputPoints[j]);
					if(Product<0)		left=true;
					else if(Product>0)	right=true;
					if(right&&left)break;
					}
				}
				if (right != left)
				{
					outputPoints.push_back(inputPoints[i]);
					outputPoints.push_back(inputPoints[j]);
					outputLines.push_back(Line(inputPoints[i],inputPoints[j]));
				}
			}
		}
		
	}
};
#endif