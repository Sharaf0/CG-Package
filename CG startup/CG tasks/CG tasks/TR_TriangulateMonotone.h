#pragma once
#ifndef MONOTONE_TRIANGULATION
#define MONOTONE_TRIANGULATION
#include "Algorithm.h"
class TR_MonotoneTriangulation : public Algorithm
{
public:
	TR_MonotoneTriangulation(){}
	Point getNext(Point p, vector<Line> v, map<Point,bool> m)
	{
		return Point();
	}
	float crossProduct(Point a, Point b, Point c)
	{
		return 0.0;
	}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines, vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		int minpointIndex = 1000;
		int minPointY = 1000;

		for(int i=0; i<inputPoints.size(); i++)
		{
			if (inputPoints[i].y < minPointY)
			{
				minpointIndex = i;
				minPointY= inputPoints[i].y;
			}
		}
		vector <Point> sortedPoints;

		sortedPoints.push_back(inputPoints[minpointIndex]);


		map<Point, bool> visited;

		for(int i=0; i<inputPoints.size(); i++)
			visited[inputPoints[i]] = false;
		visited[inputPoints[minpointIndex]] = true;

		while(sortedPoints.size() < inputPoints.size())
			sortedPoints.push_back(getNext(sortedPoints[sortedPoints.size()-1], inputLines , visited));

		vector <Point> R, L;
		int i;
		for(i=0; sortedPoints[i].y <= sortedPoints[i+1].y; i++)
			R.push_back(sortedPoints[i]);

		for(; i<sortedPoints.size(); i++)
			L.push_back(sortedPoints[i]);

		reverse(L.begin(),L.end());
		bool right;
		int LI = 0, RI = 0;

		vector <Point> q;
		q.push_back(R[RI++]);

		if(R[RI].y < L[LI].y)
		{   
			q.push_back(R[RI++]);
			right = true;
		}
		else
		{
			q.push_back(L[LI++]);
			right = false;
		}

		while(LI < L.size() && RI < R.size())
		{
			// insert new point
			if(R[RI].y < L[LI].y)
			{
				q.push_back(R[RI++]);
				if(right == false) // i was Left
				{
					// wassal el new point be kol elly fel q
					while(q.size() > 2)
					{
						outputLines.push_back(  Line(q[q.size()-1],q[0] ));
						q.erase(q.begin());
					}
					outputLines.push_back( Line(q[q.size()-1], q[0]));
					right = true;
				}
				else // R series, same series
				{
					while(q.size() > 2 && crossProduct(q[q.size()-1], q[q.size()-2], q[q.size()-3])) // TODO: replace crossProduct
					{
						outputLines.push_back( Line(q[q.size()-1], q[q.size()-3]));
						q.erase(q.begin()+q.size()-2);
					}
				}
			}
			else 
			{
				q.push_back(L[LI++]); // new point is L
				if(right == true) // if i was R
				{
					// wassal el new point be kol elly fel q
					while(q.size() > 2)
					{
						outputLines.push_back( Line(q[q.size()-1], q[0]));
						q.erase(q.begin());
					}
					outputLines.push_back(Line(q[q.size()-1], q[0
					]));
				}
				else // L series, same series
				{
					while(q.size() > 2 &&((right&& crossProduct (q[q.size()-3], q[q.size()-2], q[q.size()-1])<0 )||
						(!right&& crossProduct (q[q.size()-3], q[q.size()-2], q[q.size()-1])>0 ))) // TODO: replace crossProduct
					{
						outputLines.push_back( Line(q[q.size()-1], q[q.size()-3]));
						q.erase(q.begin()+q.size()-2);
					}
				}
			}    
		}
	}
};
#endif