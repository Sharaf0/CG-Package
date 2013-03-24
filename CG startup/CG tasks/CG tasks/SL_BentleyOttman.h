#pragma once
#ifndef SL_BENTLEYOTTMAN
#define SL_BENTLEYOTTMAN
#include "SL_Utilities.h"

#define lineIt set<pair<Point,Line>, ActiveLinesComparer>::iterator
#define mp(a,b) make_pair(a,b)
#define LEFT_END_EVENT 0
#define INTERSECTION_EVENT 1
#define RIGHT_END_EVENT 2


bool isPointLessY(const Point& a, const Point& b)
{
	if(a.y==b.y)
		return a.x > b.x;
	return a.y < b.y;
}

struct EventPointComparer
{
	bool operator()(const EventPoint* lhs, const EventPoint* rhs)const
	{
		if (lhs->currentPoint.x != rhs->currentPoint.x) 
			return lhs->currentPoint.x < rhs->currentPoint.x;
		if (lhs->currentPoint.y != rhs->currentPoint.y)
			return lhs->currentPoint.y < rhs->currentPoint.y;
		if(lhs->eventType != rhs->eventType)
			return lhs->eventType < rhs->eventType;
		return lhs->id < rhs->id;
	}
};

struct ActiveLinesComparer
{
	bool operator()(const pair<Point, Line>& lhs, const pair<Point, Line>& rhs)const
	{
		if(lhs.first == rhs.first)
			return lhs.second < rhs.second;
		return isPointLessY(lhs.first, rhs.first);
	}
};

map<Line, Point> linesRank;
set< pair<Point, Line>, ActiveLinesComparer > active;
set<EventPoint*, EventPointComparer> events;
set<Point> intersections;

void checkIntersecion(Line, Line, float, bool);
//void printSet()
//{
//	cout<<"------------------------------"<<endl;
//	for(set<EventPoint*, EventPointComparer>::iterator i = events.begin(); i != events.end(); i ++)
//	{
//		cout<<"("<<(*i)->currentPoint.x<<","<<(*i)->currentPoint.y<<")"<<endl;
//	}
//}
//void printActive()
//{
//	cout<<"Active"<<endl;
//	for(set<pair<Point,Line>,ActiveLinesComparer>::iterator i = active.begin(); i!=active.end();i++)
//		cout<<i->first<<":"<<i->second<<endl;
//}

void printIterator(lineIt l)
{
	cout<<l->first<<" "<<l->second<<endl;
}

struct LeftEndPoint : public EventPoint
{
public:
	Line currentLine;
	LeftEndPoint(Line _l)
	{
		currentLine = _l;
		currentPoint = currentLine.start;
		eventType = LEFT_END_EVENT;
		id = eventID++;
	}
	void handleTransition()
	{
		if(currentLine.isVertical())
		{
			for(lineIt i = active.begin(); i != active.end(); i ++)
				checkIntersecion(currentLine, i->second, currentPoint.x, false);
		}
		else
		{
			linesRank[currentLine] = currentPoint;
			//printActive();
			lineIt currentIterator = active.insert(mp(currentPoint, currentLine)).first;
			//printActive();
			//printIterator(currentIterator);
			lineIt before, after;
			before = after = currentIterator;
			before--;
			after ++;
			//printIterator(before);
			//printIterator(after);
			if(after!=active.end())
				checkIntersecion(after->second, currentIterator->second, currentPoint.x, true);
			if(before!=active.end())
				checkIntersecion(before->second, currentIterator->second, currentPoint.x, true);
		}
	}
};

struct RightEndPoint : public EventPoint
{
	Line currentLine;
	RightEndPoint(Line _l)
	{
		currentLine = _l;
		currentPoint = max(currentLine.start, currentLine.end);
		eventType = RIGHT_END_EVENT;
		id = eventID++;
	}
	void handleTransition()
	{
		if(currentLine.isVertical())
			return;
		Point lineRank = linesRank[currentLine];
		lineIt currentIterator = active.find(mp(lineRank,currentLine));
		lineIt before, after;
		before = after = currentIterator;
		before--; after++;
		//printActive();
		active.erase(currentIterator);
		//printActive();
		if(after!=active.end() && before!=active.end())
			checkIntersecion(before->second, after->second, currentPoint.x, true);
	}
};

struct Intersection : public EventPoint
{
	Line firstLine, secondLine;
	Intersection(Line _l1, Line _l2, Point _p)
	{
		firstLine = _l1, secondLine = _l2, currentPoint = _p;
		eventType = INTERSECTION_EVENT;
		id = eventID++;
	}
	void handleTransition()
	{
		Point firstLineRank, secondLineRank;
		firstLineRank  = linesRank[firstLine];
		secondLineRank = linesRank[secondLine];
		lineIt upper = active.find(mp(firstLineRank ,firstLine ));
		lineIt lower = active.find(mp(secondLineRank,secondLine));

		swap(linesRank[firstLine], linesRank[secondLine]);

		//printActive();
		active.erase(upper);
		//printActive();
		active.erase(lower);
		//printActive();

		upper = active.insert(mp(secondLineRank, firstLine )).first;
		//printActive();
		lower = active.insert(mp(firstLineRank , secondLine)).first;
		//printActive();

		if(isPointLessY(secondLineRank, firstLineRank))
			swap(upper, lower);

		lineIt upperOfUpper = upper, lowerOfLower = lower;
		upperOfUpper++, lowerOfLower --;

		if(upper != active.end() && upperOfUpper != active.end())
			checkIntersecion(upper->second, upperOfUpper->second, currentPoint.x, true);

		if(lower != active.end() && lowerOfLower != active.end())
			checkIntersecion(lower->second, lowerOfLower->second, currentPoint.x, true);
	}
};


void checkIntersecion(Line l1, Line l2, float curX, bool notVertical=true)
{
	Point intersectionPoint;
	if(Utilities::computeSegmentIntersection(l1.start,l1.end,l2.start,l2.end,intersectionPoint))
	{
		if(intersectionPoint.x <= curX && notVertical)
			return;
		intersections.insert(intersectionPoint);
		if(notVertical)
			events.insert(new Intersection(l1, l2, intersectionPoint));
	}
}

class SL_BentleyOttman : public Algorithm
{
public:
	SL_BentleyOttman(){}

	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		vector<Line> inputSegments = inputLines;
		EventPoint *e1, *e2;
		for(int i = 0; i < inputSegments.size(); i ++)
		{
			inputSegments[i].handleOrder();
			events.insert( new LeftEndPoint (inputSegments[i]) );
			events.insert( new RightEndPoint(inputSegments[i]) );
			//printSet();
		}
		EventPoint *e;
		while(events.size())
		{
			e = (*events.begin());
			events.erase(events.begin());
			e->handleTransition();
			//printSet();
		}

		outputPoints = vector<Point>(intersections.begin(), intersections.end());
	}
};

#endif

