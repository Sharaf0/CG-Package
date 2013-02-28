#pragma once
#ifndef CH_JARVISMARCH
#define CH_JARVISMARCH

#include "Algorithm.h"

class CH_JarvisMarch : public Algorithm
{
public:
	CH_JarvisMarch(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
						vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		if(inputPoints.size()<3)return;
		outputPoints.clear(), outputLines.clear();

		//vector<Point>::iterator minXPointIndex;
		//min_element(inputPoints.begin(),inputPoints.end());


		//Line initialVector(Point(minXPoint.x,0),Point(minXPoint.x,-1));
		//float minAngle = 1000;
		//int minAngleIndex;
		//for(unsigned i = 0; i < inputPoints.size(); i ++)
		//{
		//	Utilities::getAngle2Vectors(Line,
		//}
	}
};
#endif