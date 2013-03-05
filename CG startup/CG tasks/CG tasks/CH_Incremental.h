#pragma once
#ifndef CH_INCREMENTAL
#define CH_INCREMENTAL

#define PointIterator set<Point, AngleComparer>::iterator
#include "Algorithm.h"

class CH_Incremental : public Algorithm
{
private:
	bool outside(const set<Point,AngleComparer>& convex, const PointIterator up, const PointIterator down, Point p)
	{
		if(up == convex.end() || down == convex.end())
			return true;
		return Utilities::crossProduct(p,(*up),(*down))>0;
	}
	void addPoint(set<Point, AngleComparer>& convex, PointIterator up, PointIterator down, const Point& p)
	{
		//upper
		if(up!=convex.end())
		{
			PointIterator t = up;
			t++;
			while(convex.size()>=2 && t!=convex.end() && Utilities::crossProduct(p,(*up),(*t)) < 0)
				up = convex.erase(up), t ++;
		}
		//lower
		if(down!=convex.begin())
		{
			PointIterator t = down;
			t --;
			while(convex.size()>=2 && t!=convex.begin() && Utilities::crossProduct(p,(*down),(*t)) > 0)
				down = --(convex.erase(down)), t --;
		}
		convex.insert(p);
	}
public:
	CH_Incremental (){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		outputPoints.clear(), outputLines.clear();
		vector<Point> input = inputPoints;

		if(input.size()<3)
			return;

		int n = input.size();

		double zero = 0.0;
		Point origin(1 / zero, 1 / zero);//OO
		int index;
		for (int i = 0; i < input.size(); i++)
			if (make_pair(input[i].y, input[i].x) < make_pair(origin.y, origin.x))
				origin = input[i], index = i;

		swap(input[0],input[index]);//let the origin always be at input[0]
		AngleComparer::about = origin;

//		Point maxPoint, minPoint;
//		AngleComparer anglecomparer;
//		maxPoint = (*max_element(input.begin(), input.end(), anglecomparer));
//		minPoint = (*min_element(input.begin(), input.end(), anglecomparer));

		
		//input.erase(input.begin()+index);

		set<Point, AngleComparer> convex;
		
//		convex.insert(origin);
//		convex.insert(minPoint);
//		convex.insert(maxPoint);

		for(int i = 0; i < 3; i ++)
			convex.insert(input[i]);

		PointIterator upIt, downIt;
		for(int i = 3; i < n; i ++)
		{
			upIt   = convex.upper_bound(input[i]);
			downIt = upIt;
			downIt--;
			if(!outside(convex, upIt, downIt, input[i]))
				continue;
			//INSIDE
			addPoint(convex, upIt, downIt,input[i]);
		}
		for(PointIterator i = convex.begin(); i!=convex.end(); i ++)
			outputPoints.push_back((*i));
		outputPoints.push_back(AngleComparer::about);
		for(int i = 0; i < outputPoints.size(); i ++)
			outputLines.push_back(Line(outputPoints[i],outputPoints[(i+1)%outputPoints.size()]));
		if(outputPoints.size() < 3)
			outputPoints.clear(), outputLines.clear();
	}
};
#endif