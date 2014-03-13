#pragma once
#ifndef MONOTONE_TRIANGULATION
#define MONOTONE_TRIANGULATION
#include "Algorithm.h"
class TR_MonotoneTriangulation : public Algorithm
{
private:


public:
	TR_MonotoneTriangulation(){}

	bool Equals(Line l1, Line l2)
	{
		return (l1.start == l2.start && l1.end == l2.end)
			|| (l1.end == l2.start && l1.start == l2.end);
	}

	Point getNext(Point p, vector <Line>  lines, map<Point, bool>& visited)
	{
		vector <Point> res;
		for(int i=0; i<lines.size(); i++)
		{
			if(lines[i].start == p && visited[lines[i].end] == false)
			{
				res.push_back(lines[i].end);

			}
			if(lines[i].end == p && visited[lines[i].start] == false)
			{
				res.push_back(lines[i].start);

			}
		}

		if(res.size() == 2) // this case will be true only for first call
		{
			if ( res[0] > res[1] )
			{
				visited[res[0]] = true;
				return res[0];
			}
			else
			{
				visited[res[1]] = true;
				return res[1];
			}
		}
		visited[res[0]] = true;
		return res[0];
	}


	void run(const vector<Point>& inputPoints_, const vector<Line>& inputLines, vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		vector<Point>  inputPoints;
		for (int i = 0 ; i < inputLines.size() ; i++)
		{
			inputPoints.push_back(Point(inputLines[i].start));
		}

		//unique(inputPoints.begin(), inputPoints.end());

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
		for(i=0; sortedPoints[i].y < sortedPoints[i+1].y; i++)
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

		while(LI < L.size() || RI < R.size())
		{
			// insert new point
			if(RI<R.size() && R[RI].y < L[LI].y)
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
					while(q.size() > 2 && (Utilities::crossProduct (q[q.size()-3], q[q.size()-2], q[q.size()-1])>0 ))
					{
						outputLines.push_back( Line(q[q.size()-1], q[q.size()-3]));
						q.erase(q.begin()+q.size()-2);
					}
				}
			}
			else if(LI < L.size())// new point is L
			{
				q.push_back(L[LI++]); 
				if(right == true) // if i was R
				{
					// wassal el new point be kol elly fel q
					while(q.size() > 2)
					{
						outputLines.push_back( Line(q[q.size()-1], q[0]));
						q.erase(q.begin());
					}
					outputLines.push_back(Line(q[q.size()-1], q[0]));
					right = false;
				}
				else // L series, same series
				{
					while(q.size() > 2 && (Utilities::crossProduct (q[q.size()-3], q[q.size()-2], q[q.size()-1])<0 )) // TODO: replace crossProduct
					{
						outputLines.push_back( Line(q[q.size()-1], q[q.size()-3]));
						q.erase(q.begin()+q.size()-2);
					}
				}
			}    
		}

		for(int i=0; i<outputLines.size(); i++)
		{
			for(int j=0; j<inputLines.size(); j++)
				if(Equals(outputLines[i], inputLines[j]))
					outputLines.erase(outputLines.begin()+i);
		}
	}

};
#endif