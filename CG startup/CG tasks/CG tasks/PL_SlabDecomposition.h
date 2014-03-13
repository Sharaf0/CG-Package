#pragma once
#ifndef PL_SLABDECOMPOSITION
#include "Algorithm.h"
void drawLine(Line l)
{
	glBegin(GL_LINES);
	glColor3f(1,1,0);
	glLineWidth(5.0);
	glVertex2f(l.start.x, l.start.y);
	glVertex2f(l.end.x, l.end.y);
	glEnd();
	glFlush();
}
Line getSweep(float x)
{
	return Line(Point(x, -10000), Point(x, 10000));
}
Point SD_currentPoint;
struct SD_ActiveLineComparer
{
public:
	bool operator() (const Line lhs, const Line rhs)const
	{
		Line l = getSweep(SD_currentPoint.x);
		Point lhsIntersection, rhsIntersection;

		Utilities::computeSegmentIntersection(l.start, l.end, lhs.start, lhs.end, lhsIntersection);
		Utilities::computeSegmentIntersection(l.start, l.end, rhs.start, rhs.end, rhsIntersection);

		if(lhsIntersection.y != rhsIntersection.y)
			return lhsIntersection.y < rhsIntersection.y;
		else
			return lhs.end.y < rhs.end.y;
	}
};
struct Slab
{
	float currentX;
	set<Line,SD_ActiveLineComparer> lines;
	Slab(){}
	Slab(float x)
	{currentX = x;}
};
struct SD_SlabComparer
{
public:
	bool operator() (const Slab* lhs, const Slab* rhs)const
	{return lhs->currentX < rhs->currentX;}
};
bool inSlab(Line l, Slab* s)
{
	return l.start.x <= s->currentX && l.end.x > s->currentX;
}
void getLocation(Point p, set<Slab*, SD_SlabComparer>slabs)
{
	Line before, after;
	Slab* s = new Slab(p.x);
	set<Slab*, SD_SlabComparer>::iterator it =  slabs.lower_bound(s);

	if(it==slabs.begin()||it==slabs.end())
	{
		if(it==slabs.begin())
			drawLine(getSweep((*it)->currentX));
		else
		{
			it--;
			drawLine(getSweep((*it)->currentX));
		}
		return;
	}

	drawLine(getSweep((*it)->currentX));
	it--;
	after = getSweep((*it)->currentX);
	drawLine(getSweep((*it)->currentX));

	SD_currentPoint = p;

	Line ourPoint(Point(-10000,p.y),Point(10000,p.y));
	set<Line,SD_ActiveLineComparer>::iterator lineSlab = (*it)->lines.insert(ourPoint).first;
	set<Line,SD_ActiveLineComparer>::iterator regionBefore, regionAfter;
	regionAfter = regionBefore = lineSlab;
	regionAfter ++, regionBefore --;
	if(regionBefore==(*it)->lines.end() || regionAfter==(*it)->lines.end())
	{
		if(regionAfter==(*it)->lines.end())
		{
			//regionBefore --;
			drawLine((*regionBefore));
		}
		else
		{
			//regionAfter ++;
			drawLine((*regionAfter));
		}
		return;
	}
	drawLine((*regionBefore));
	drawLine((*regionAfter));

	//	Utilities::computeSegmentIntersection(Point(-10000, p.y), Point(10000, p.y), before.start, before.end, ourPoint.start);
	//	Utilities::computeSegmentIntersection(Point(-10000, p.y), Point(10000, p.y), after.start, after.end, ourPoint.end);
	//set<Line,SD_ActiveLineComparer>::iterator lineSlab = (*it)->lines.lower_bound(ourPoint);

}

class PL_SlabDecomposition : public Algorithm
{
public:
	PL_SlabDecomposition(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		if(!inputLines.size())
			return;

		vector<Line> input = inputLines;
		for(int i = 0; i < inputLines.size(); i ++)
			input[i].handleOrder();
		sort(input.begin(), input.end());

		set<Slab*, SD_SlabComparer> slabs;
		Slab* tempSlab;

		for(int i = 0; i < inputLines.size(); i ++)
		{
			tempSlab = new Slab(input[i].start.x);
			slabs.insert(tempSlab);
			tempSlab = new Slab(input[i].end.x);
			slabs.insert(tempSlab);
		}
		Point start,end;
		Line sweepLine;
		for(set<Slab*, SD_SlabComparer>::iterator i = slabs.begin(); i != slabs.end(); i++)
		{
			for(int j = 0; j < input.size(); j ++)
				if(inSlab(input[j], (*i) ))
				{
					sweepLine = Line(Point((*i)->currentX, -10000), Point((*i)->currentX, 10000));
					SD_currentPoint = Point((*i)->currentX, -10000);
					Utilities::computeSegmentIntersection(sweepLine.start,sweepLine.end,input[j].start,input[j].end,start);
					input[j].start = start;


					//{
					//	set<Slab*, SD_SlabComparer>::iterator temp = i;
					//	temp ++;
					//	if(temp != slabs.end())
					//	{
					//		sweepLine = Line(Point((*temp)->currentX, -10000), Point((*temp)->currentX, 10000));
					//		Utilities::computeSegmentIntersection(sweepLine.start,sweepLine.end,input[j].start,input[j].end,end);
					//		input[j].end = end;
					//	}
					//}


					(*i)->lines.insert(input[j]);
				}
				drawSweep((*i)->currentX);
		}

		for(int i = 0; i < inputPoints.size(); i ++)
		{
			getLocation(inputPoints[i],slabs);
		}

	}
};
#endif