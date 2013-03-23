#pragma once
#ifndef SL_UTILITIES
#define SL_UTILITIES
#include "Algorithm.h"

struct EventPoint
{
public:
	Point currentPoint;
	int eventType;//used to sort
	int id;
	static int eventID;
	virtual void handleTransition()=0;
	
};

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

#endif
