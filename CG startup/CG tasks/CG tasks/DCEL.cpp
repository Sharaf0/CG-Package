#pragma once
#ifndef DCEL_CLASS
#define DCEL_CLASS
#include "Point.h"
#include "Line.h"

class DCEL
{
	struct DCEL_Point
	{
		Point p;
		DCEL_Point* prev;
		DCEL_Point* next;
		DCEL_Point(){}
		DCEL_Point(Point _p)
		{
			p = _p;
			next = prev = NULL;
		}
	};
	struct DCEL_Edge
	{
		DCEL_Point *start, *end;
		//SmartEdge* twin;
		DCEL_Edge *next, *prev;
	};
	struct DCEL_Face
	{

	};
};

#endif
