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
		PointIterator t = up;
		if(t!=convex.end())
			t++;
		while(convex.size()>=2 && t!=convex.end() && up!=convex.end() && Utilities::crossProduct(p,(*up),(*t)) < 0)
			up = convex.erase(up);
		//lower
		t = down;
		if(up!=convex.begin())
			t --;
		while(convex.size()>=2 && up!=convex.begin()&& down!=convex.begin()&& Utilities::crossProduct(p,(*down),(*t)) > 0)
			down = convex.erase(down);
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
		swap(input[0],input[index]);
		AngleComparer::about = origin;
		//input.erase(input.begin()+index);

		set<Point, AngleComparer> convex;
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
			if(upIt==convex.begin())
				addPoint(convex, upIt, convex.end(),input[i]);
			else
				addPoint(convex, upIt, downIt,input[i]);
		}
		for(PointIterator i = convex.begin(); i!=convex.end(); i ++)
			outputPoints.push_back((*i));
		outputPoints.push_back(AngleComparer::about);
		for(int i = 0; i < outputPoints.size(); i ++)
			outputLines.push_back(Line(outputPoints[i],outputPoints[(i+1)%outputPoints.size()]));
	}
};
#endif