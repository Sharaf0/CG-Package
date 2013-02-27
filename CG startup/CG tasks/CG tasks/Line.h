#pragma once
#ifndef LINE
#define LINE

#include "Point.h"
struct Line
{
	Point start, end;
	Line(){}
	Line(Point startPoint, Point endPoint): start(startPoint), end(endPoint)
	{}

};
#endif