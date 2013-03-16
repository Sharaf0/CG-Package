#pragma once
#ifndef POINT
#define POINT
class Point
{
public:
	static int drawID;
	int pointDrawID;
	float x, y;
	Point(){}
	Point(float _x, float _y, bool toDraw=false): x(_x), y(_y)
	{
		if(toDraw) pointDrawID = drawID++;
		else pointDrawID = 0;
	}
	//based on x, if tie then y
	bool Point::operator <(const Point &p) const
	{return x < p.x || (x == p.x && y < p.y);}

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

};
#endif