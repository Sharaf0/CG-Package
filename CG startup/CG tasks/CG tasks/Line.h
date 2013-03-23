#pragma once
#ifndef LINE
#define LINE

#include "Point.h"
class Line
{
public:
	static int drawID;
	int lineDrawID;
	Point start, end;
	Line(){}
	Line(Point startPoint, Point endPoint, bool toDraw=false, bool ordered=false): start(startPoint), end(endPoint)
	{
		if(toDraw) lineDrawID = drawID++;
		else lineDrawID = 0;

		if(ordered)
		{
			start	= startPoint < endPoint ? startPoint : endPoint;
			end		= startPoint > endPoint ? startPoint : endPoint;
		}
	}
	bool operator == (const Line& l)const
	{
		return this->start==l.start&&this->end==l.end;
	}
	bool isVertical()const
	{
		return start.x==end.x;
	}
	void handleOrder()
	{
		Point startPoint = start, endPoint = end;
		start	= startPoint < endPoint ? startPoint : endPoint;
		end		= startPoint > endPoint ? startPoint : endPoint;
	}
	bool operator < (const Line& l)const
	{
		return this->lineDrawID < l.lineDrawID;
	}
	friend std::ostream & operator<<(std::ostream &os, const Line& l)
	{
		os << l.start << " " << l.end;
		return os;
	}
};
#endif