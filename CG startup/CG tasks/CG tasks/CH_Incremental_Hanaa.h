#pragma once
#ifndef CH_INCREMENTAL
#define CH_INCREMENTAL

#include "Algorithm.h"


class CH_Incremental_Hanaa : public Algorithm
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

	bool isExterior (vector <Point> Polygon , Point point)
	{
		bool left= false, right =false;
		for (int i = 0 ; i < Polygon.size()-1 ; i++)
		{
			float product = Utilities::crossProduct(Polygon[i],Polygon[i+1],point);
			if (product >0 ) left = true;
			else right = true;
		}

		float product = Utilities::crossProduct(Polygon[Polygon.size()-1],Polygon[0],point);
		if (product >0 ) left = true;
		else right = true;

		if (left&&right) return true;
	}

public:
	CH_Incremental_Hanaa(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{

	}
};
#endif