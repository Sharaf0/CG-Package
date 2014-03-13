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

		//Handle Colinear points

		Point minYPoint(10000,10000);
		int minYPointIndex;
		for(unsigned i = 0; i < input.size(); i ++)
		{
			if(isLessY(input[i],minYPoint))
				minYPoint = input[i],
				minYPointIndex = i;
		}

		Line movingVector(Point(minYPoint.x-1,minYPoint.y),minYPoint);

		float minAngle, angleNow;
		int minAngleIndex;

		outputPoints.push_back(minYPoint);
		while(outputPoints[0]!=outputPoints[outputPoints.size()-1] || outputPoints.size()==1)
		{
			minAngle = 1000;
			minAngleIndex = -1;
			for(unsigned i = 0; i < input.size(); i ++)
			{
				if(input[i] == movingVector.end)
					continue;
				angleNow = Utilities::getAngle2Vectors(movingVector.start, movingVector.end, movingVector.end, input[i]);
				if(angleNow < minAngle)
					minAngle = angleNow,
					minAngleIndex = i;
				else if(angleNow == minAngle)
				{
					if(!Utilities::pointOnSegment(movingVector.end, input[minAngleIndex], input[i]))
						minAngleIndex = i;
				}
			}
			//if(minAngleIndex==-1)//All points are colinear
			//{
			//	outputPoints.pop_back();//pop extreme point
			//	return;
			//}
			movingVector = Line(movingVector.end,input[minAngleIndex]);
			outputPoints.push_back(input[minAngleIndex]);
			outputLines.push_back(movingVector);
		}
	}
};
#endif