#pragma once
#ifndef CH_GRAHAM
#define CH_GRAHAM

#include "Algorithm.h"




class CH_Graham_Hanaa : public Algorithm
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
	CH_Graham_Hanaa(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{

		if (inputPoints.size() <2 ) {outputPoints = inputPoints; return;}
		outputPoints.clear(), outputLines.clear();

		//Get The Min Point , 3shan adman enha fel CH
		Point minYPoint(10000,10000);
		int minYPointIndex;
		for(unsigned i = 0; i < inputPoints.size(); i ++)
		{
			if(isLessY(inputPoints[i],minYPoint))
				minYPoint = inputPoints[i],
				minYPointIndex = i;
		}

		outputPoints.push_back(minYPoint);

		AnglePointQueue PointsSorted ;
		PointsSorted = Utilities::sortByAngle(inputPoints,minYPointIndex);
		
		priority_queue  <AnglePoint , vector <AnglePoint> , less<AnglePoint> > PointsSortedAngularly = PointsSorted.Queue;
		outputPoints.push_back(PointsSortedAngularly.top().P);
		PointsSortedAngularly.pop();

		while (!PointsSortedAngularly.empty())
		{
			Point P1 = outputPoints[outputPoints.size()-1];			
			Point P2 = outputPoints[outputPoints.size()-2];

			float Product = Utilities::crossProduct(P2,P1,PointsSortedAngularly.top().P);
			if (Product > 0)
			{
				outputPoints.push_back(PointsSortedAngularly.top().P);
				PointsSortedAngularly.pop();
			}
			else
			{
				outputPoints.erase(outputPoints.end()-1);
			}
		}
		for (int i = 0 ; i < outputPoints.size()-1;i++)
		{
			outputLines.push_back(Line(outputPoints[i],outputPoints[i+1]));
		}
		outputLines.push_back(Line(outputPoints[outputPoints.size()-1],outputPoints[0]));
	}
};
#endif