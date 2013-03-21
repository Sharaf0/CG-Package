#pragma once
#ifndef SL_UTILITIES
#define SL_UTILITIES
#include "Algorithm.h"

#define intersect(l1,l2,p) Utilities::computeSegmentIntersection(l1.start,l1.end,l2.start,l2.end,p)
#define lineIt set<Line*>::iterator

struct EventPoint
{
public:
	Point p;
	virtual void handleTransition(map<Line*>&, map<EventPoint*>&, list<EventPoint*>*)=0;
};

struct LeftEndPoint : public EventPoint
{
public:
	Line l;
	LeftEndPoint(Line* _l)
	{
		l = *_l;
		p = min(l.start,l.end);
	}
	void handleTransition(set<Line*>& sweepLine, set<EventPoint*>& events, list<EventPoint*>* result)
	{
		sweepLine.insert(&l);
		lineIt current = sweepLine.find(&l);
		lineIt before, after;
		before = after = current;
		before--; after++;
		Point p;
		if(intersect((*after) , (*current), p) )
			events.insert(Intersection((*current), (*after), p));
		if(intersect((*before), (*current), p) )
			events.insert(Intersection((*current), (*before), p));
	}
};

struct RightEndPoint : public EventPoint
{
	Line l;
	RightEndPoint(Line *_l)
	{
		l = *_l;
		p = max(l.start, l.end);
	}
	void handleTransition(set<Line*>& sweepLine, set<EventPoint*>& events, list<EventPoint*>* result)
	{
	}
};

struct Intersection : public EventPoint
{
	Line l1, l2;
	Intersection(Line *_l1, Line *_l2, Point &_p)
	{
		l1 = *_l1, l2 = *_l2, p = _p;
	}
	void handleTransition(map<Line*>&, map<EventPoint*>&, list<EventPoint*>*)
	{
	}
};
#endif
