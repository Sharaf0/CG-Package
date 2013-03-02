#pragma once
#ifndef CH_QUICKHULL
#define CH_QUICKHULL

#include "Algorithm.h"

class CH_QuickHull : public Algorithm
{
private:
	vector<Point>quickHull(int minPoint, int maxPoint, vector<Point> S)
	{
		Line v1, v2, v3;
		//if(
		return vector<Point>();
	}
public:
	CH_QuickHull(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
						vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		if(inputPoints.size()<3)return;
		outputPoints.clear(), outputLines.clear();
		
		int minPointIndex, maxPointIndex;
		Point minPoint( 10000, 10000);
		Point maxPoint(-10000,-10000);

		for(unsigned i = 0; i < inputPoints.size(); i ++)
		{
			if(inputPoints[i]<minPoint)
				minPoint = inputPoints[i],
				minPointIndex = i;

			if(maxPoint<inputPoints[i])
				maxPoint = inputPoints[i],
				maxPointIndex = i;
		}
		vector<Point> upPoints;
		vector<Point> downPoints;

		for(unsigned i = 0; i < inputPoints.size(); i ++)
		{
			float cross = Utilities::crossProduct(inputPoints[i], minPoint,maxPoint);
			if(cross<0)//left
				upPoints.push_back(inputPoints[i]);
			if(cross>0)//right
				downPoints.push_back(inputPoints[i]);
		}
	}
};
#endif