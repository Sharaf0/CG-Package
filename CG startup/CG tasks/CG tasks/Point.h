#pragma once
#ifndef POINT
#define POINT

struct Point
{
	float x, y;
	Point(){}
	Point(float _x, float _y): x(_x), y(_y)
	{}
	bool Point::operator <(const Point &p) const
	{return x < p.x || (x == p.x && y < p.y);}
	bool operator ==(const Point &p)const
	{return p.x==this->x && p.y==this->y;}
	bool operator !=(const Point &p)const
	{return (p.x!=this->x || p.y!=this->y); }

};
#endif