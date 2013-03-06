#pragma once
#ifndef CH_DIVIDE_AND_CONQUER
#define CH_DIVIDE_AND_CONQUER
#include "Algorithm.h"
typedef set<Point, AngleComparer> PointsSet;
#define PointIterator set<Point, AngleComparer>::iterator
class CH_DivideAndConquer : public Algorithm
{
	PointIterator getMinX(PointsSet S)
	{
		float z = 0;
		Point mn(1/z,1/z);
		PointIterator j;
		for(PointIterator i = S.begin(); i != S.end(); i ++)
			if((*i) < mn)
				mn = (*i),
				j = i;
		return j;
	}
public:
	CH_DivideAndConquer(){}
	vector<Point> merge(PointsSet S1, PointsSet S2)
	{
		PointIterator a = getMinX(S1);//right most
		PointIterator b = getMinX(S2);//left most

		PointIterator	aUpper, bUpper,
						aLower, blower;
		PointIterator next;
		//upper tangent
		{
		}
		//lower tangent
		{
		}
	}
	vector<Point> getConvex(vector<Point> S)
	{
		if(S.size()<4)
			return S;
		vector<Point> left, right;
		for(int i = 0; i < S.size(); i ++)
			if(i<S.size()/2)
				left.push_back(S[i]);
			else
				right.push_back(S[i]);
		left = getConvex(left);
		right = getConvex(right);

		set<Point, AngleComparer> S1, S2;
		for(int i = 0; i < left.size() ; i ++)S1.insert(left[i]);
		for(int i = 0; i < right.size(); i ++)S2.insert(right[i]);

		return merge(S1, S2);
	}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		vector<Point> input = inputPoints;
		sort(input.begin(), input.end());
		//outputPoints = getConvex(input);

	}
};
#endif