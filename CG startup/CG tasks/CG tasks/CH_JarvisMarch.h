#pragma once
#ifndef CH_JARVISMARCH
#define CH_JARVISMARCH

#include "Algorithm.h"

class CH_JarvisMarch : public Algorithm
{
private:
	bool isLessY(const Point& a, const Point& b)const
	{
			if(a.y<b.y)return true;
			if(a.y>b.y)return false;
			//a.y==b.y
			if(a.x<b.x)return true;
			//a.x>b.x
			return false;
	}
public:
	CH_JarvisMarch(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
						vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		if(inputPoints.size()<3)return;
		outputPoints.clear(), outputLines.clear();

		//Handle Colinear points
		
		Point minYPoint(10000,10000);
		int minYPointIndex;
		for(unsigned i = 0; i < inputPoints.size(); i ++)
		{
			if(isLessY(inputPoints[i],minYPoint))
				minYPoint = inputPoints[i],
				minYPointIndex = i;
		}

		Line movingVector(Point(minYPoint.x+1,minYPoint.y),minYPoint);
		
		float minAngle, angleNow;
		int minAngleIndex;

		outputPoints.push_back(minYPoint);
		while(outputPoints[0]!=outputPoints[outputPoints.size()-1] || (outputPoints.size()==1&&outputPoints[0]==outputPoints[outputPoints.size()-1]))
		{
			minAngle = 1000;
			for(unsigned i = 0; i < inputPoints.size(); i ++)
			{
				angleNow = Utilities::getAngle2Vectors(movingVector.start,movingVector.end,movingVector.end,inputPoints[i]);
				if(angleNow<minAngle)
					minAngle = angleNow,
					minAngleIndex = i;
			}
			movingVector = Line(movingVector.end,inputPoints[minAngleIndex]);
			outputPoints.push_back(inputPoints[minAngleIndex]);
			outputLines.push_back(movingVector);
		}
	}
};
#endif