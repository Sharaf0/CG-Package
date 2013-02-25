#ifndef LINE_H
#define LINE_H
#include "Point.h"
struct Line
{
	Point start, end;
	Line(){}
	Line(Point startPoint, Point endPoint): start(startPoint), end(endPoint)
	{}

};
#endif