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

#endif
