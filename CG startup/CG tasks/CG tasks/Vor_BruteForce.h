#pragma once
#ifndef VOR_BRUTEFORCE
#define VOR_BRUTEFORCE

#include "Algorithm.h"
#include <complex>
//#include "quadtree.h"
typedef complex<double> point;
typedef pair<point,point> segment;

int maxX=1000;
int maxY=1000;
int minX=0;
int minY=0;
#define eps 1e-8
#define X real()
#define Y imag()
class VOR_BruteForce : public Algorithm
{
public:
	VOR_BruteForce (){}
	long double cross_product(const point &a,const point &b)
	{
		return imag(conj(a)*b);
	}
	point vec(const point &a,const point &b)
	{
		return b-a;
	}
	int comp(long double a, long double b)
	{
		if( fabs(a-b) < eps ) return 0;
		return (a < b ? -1 : 1);
	}
	// returns intersection of infinite lines ab and pq (undefined if they are parallel)
	bool intersect(const point &a, const point &b, const point &p, const point &q,point &ret)
	{
		double d1 = cross_product(p - a, b - a);
		double d2 = cross_product(q - a, b - a);
		ret = (d1 * q - d2 * p) / (d1 - d2);
		return fabs(d1-d2) > eps;
	}

	void cut(const point &A,const point &B, vector<segment> &poly)
	{
		vector<segment> res;
		bool cuts = false;
		point cutA, cutB;
		for(int i=0;i<poly.size();i++)
		{
			segment s = poly[i];
			bool in1 = cross_product(vec(A,B),vec(A,s.first))>=0;
			bool in2 = cross_product(vec(A,B),vec(A,s.second))>=0;
			if(in1 && in2) res.push_back(s);
			else if(in1)
			{
				point p;
				intersect(s.first,s.second,A,B,p);
				if(!(comp(s.first.X,p.X)==0 && comp(s.first.Y,p.Y)==0)) res.push_back(segment(s.first,p));
				cuts=true;
				cutA = p;
			}
			else if(in2)
			{
				point p;
				intersect(s.first,s.second,A,B,p);
				if(!(comp(p.X,s.second.X)==0 && comp(p.Y,s.second.Y)==0)) res.push_back(segment(p,s.second));
				cuts=true;
				cutB = p;
			}
		}
		if (cuts && !(comp(cutA.X,cutB.X)==0 && comp(cutA.Y,cutB.Y)==0)) res.push_back(segment(cutA, cutB));
		poly = res;
	}

	void calc(int nr,vector<point> &points,vector<segment> &poly)
	{
		poly.clear();
		poly.push_back(segment(point(minX,minY), point(maxX,minY)));
		poly.push_back(segment(point(maxX,minY), point(maxX,maxY)));
		poly.push_back(segment(point(maxX,maxY), point(minX,maxY)));
		poly.push_back(segment(point(minX,maxY), point(minX,minY)));
		for(int i = 0; i < points.size(); i ++)
			if(i!=nr)
			{
				point m = 0.5*(points[nr]+points[i]);
				point v = points[i]-points[nr];
				point w = point(-v.Y, v.X);
				point n = m + w;
				cut(m,n,poly);
			}
	}

	void runVoronoi(vector<point> &points,vector<vector<segment> > &polys)
	{
		vector<segment> poly;
		for(int i = 0; i < points.size(); i ++)
		{
			calc(i,points,poly);
			polys.push_back(poly);
		}
	}

	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		vector<point> v(inputPoints.size());
		for(int i = 0; i < inputPoints.size(); i ++)
			v[i] = point(inputPoints[i].x, inputPoints[i].y);
		vector<vector<segment> > a;
		runVoronoi(v,a);
		//ofstream outFile("C:\\Users\\Sh\\Desktop\\o.txt");
		for(int i = 0; i < a.size(); i ++)
			for(int j = 0; j < a[i].size(); j ++)
			{
				Line l(Point(a[i][j].first.X, a[i][j].first.Y), Point(a[i][j].second.X, a[i][j].second.Y));
				outputLines.push_back(l);
				//outFile<<l;
			}
			//outFile.close();
			int x = 7;
	}
};
#endif