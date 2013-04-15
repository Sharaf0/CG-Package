#pragma once
#ifndef UTILITIES
#define UTILITIES

#include "Line.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <set>
#include <list>
#include <map>
#include <queue>

using namespace std;

#define PI (2*acos(0.0))
#define EPS 1e-9
class Utilities
{
public:
	static float dot2Vectors	(const Point& p1, const Point& p2)
	{
		return p1.x*p2.x+p1.y*p2.y;
	}
	static float magnitude		(const Point& p)
	{
		return hypot(p.x,p.y);
	}
	static Point normalize		(const Point& p)
	{
		return p/magnitude(p);
	}
	static float crossProduct	(const Point& p1, const Point& p2, const Point& p3)
	{
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}
	static float cross2Vectors	(const Point& a, const Point& b)
	{
		return a.x*b.y - a.y*b.x;
	}
	static bool PointInTriangle	(const Point& p, const Point& v1, const Point& v2, const Point& v3)
	{
		bool b1, b2, b3;
		float f1, f2, f3;

		f1 = Utilities::crossProduct(p, v1, v2);
		f2 = Utilities::crossProduct(p, v2, v3);
		f3 = Utilities::crossProduct(p, v3, v1);

		if(f1==0&&f2==0&&f3==0)//If all points are collinear
			return false;

		b1 = f1<0.0f;
		b2 = f2<0.0f;
		b3 = f3<0.0f;
		return ((b1 == b2) && (b2 == b3));
	}
	static float getAngle2Vectors	 (const Point& a, const Point& b, const Point& p, const Point& q)
	{
		Point v1 = Point(b.x-a.x,b.y-a.y);
		Point v2 = Point(q.x-p.x,q.y-p.y);
		//Todo: Handle Zero Division
		float t = (v1.x*v2.x + v1.y*v2.y)/(hypot(v1.x,v1.y)*hypot(v2.x,v2.y));
		return acos( t )*(180/PI);
	}
	static float getAngle2Vectors	 (const Line& v1, const Line& v2)
	{
		return getAngle2Vectors(v1.start,v1.end,v2.start,v2.end);
	}
	static float getLinePointDistance(const Line& l, const Point& p)
	{
		Point v(l.end.x - l.start.x, l.end.y - l.start.y);
		if(v.x+v.y==0)
			return hypot(l.start.x - p.x, l.start.y - p.y);

		return fabs(crossProduct(p,l.start,l.end)/hypot(v.x,v.y));
	}
	static float dist2(Point p, Point q)
	{
		return dot2Vectors(p-q,p-q);
	}//Don't know its exact target..
	static bool linesParallel(Point a, Point b, Point c, Point d)
	{
		return fabs(cross2Vectors(b-a, c-d)) < EPS;
	}
	static bool linesCollinear(Point a, Point b, Point c, Point d)
	{
		return linesParallel(a, b, c, d) && fabs(cross2Vectors(a-b, a-c)) < EPS	&& fabs(cross2Vectors(c-d, c-a)) < EPS; 
	}
	static bool checkSegmentsIntersect(Point a, Point b, Point c, Point d)
	{
		if (linesCollinear(a, b, c, d))
		{
			if (dist2(a, c) < EPS || dist2(a, d) < EPS ||
				dist2(b, c) < EPS || dist2(b, d) < EPS)
					return true;
			if (dot2Vectors(c-a, c-b) > 0 &&
				dot2Vectors(d-a, d-b) > 0 &&
				dot2Vectors(c-b, d-b) > 0)
					return false;
			return true;
		}
		if (cross2Vectors(d-a, b-a) * cross2Vectors(c-a, b-a) > 0) return false;
		if (cross2Vectors(a-c, d-c) * cross2Vectors(b-c, d-c) > 0) return false;
		return true;
	}
	static Point ComputeLineIntersection(Point a, Point b, Point c, Point d)
	{
		b=b-a; d=c-d; c=c-a;
		return a + b*cross2Vectors(c, d)/cross2Vectors(b, d);
	}
	static bool computeSegmentIntersection(Point a, Point b, Point c, Point d, Point& ret)
	{
		if(!checkSegmentsIntersect(a, b, c, d))
			return false;
		ret = ComputeLineIntersection(a, b, c, d);
		return true;
	}
	static bool pointOnRay(const Point& a, const Point& b, const Point& p)
	{
		if(a==b)
			return a==p;
		Point v1 = normalize(b-a);
		Point v2 = normalize(p-a);
		return v1 == v2;
	}
	static bool pointOnSegment(const Point& a, const Point& b, const Point& p)
	{
        if (a == b)
                return a==p;
        return pointOnRay(a, b, p) && pointOnRay(b, a, p);
	}
};
struct AngleComparer
{
	static Point about;
	AngleComparer(){}
	AngleComparer(Point c) {about = c;}
	bool operator()(const Point& p, const Point& q) const
	{
		double cr = Utilities::crossProduct(about,p,q);
		if (fabs(cr) < EPS)
			return make_pair(p.y, p.x) < make_pair(q.y, q.x);
		return cr > 0;
	}
};
#endif