#pragma once
#ifndef POINT
#define POINT
#include <iostream>
class Point
{
public:
	float x, y;
	int pointDrawID;
	static int drawID;
	Point(){}
	Point(float _x, float _y, bool toDraw=false): x(_x), y(_y)
	{
		if(toDraw) pointDrawID = drawID++;
		else pointDrawID = 0;
	}
	//based on x, if tie then y
	bool operator <(const Point &p) const
	{return x < p.x || (x == p.x && y < p.y);}

	bool operator > (const Point& p) const
	{return x > p.x || (x == p.x && y > p.y);}

	Point operator + (const Point &p) const
	{return Point(x+p.x, y+p.y);}

	Point operator - (const Point &p) const
	{return Point(x-p.x, y-p.y);}

	Point operator * (float c) const
	{return Point(x*c,y*c);}

	Point operator / (float c) const
	{ return Point(x/c,y/c);}

	bool operator ==(const Point &p)const
	{return p.x==this->x && p.y==this->y;}
	
	bool operator !=(const Point &p)const
	{return (p.x!=this->x || p.y!=this->y); }

	friend std::ostream & operator<<(std::ostream &os, const Point& p)
	{
		os << "(" << p.x << "," << p.y << ")";
		return os;
	}

};
#endif