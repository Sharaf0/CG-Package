#pragma once
#ifndef INSERTING_DIAGONALS
#define INSERTING_DIAGONALS
#include "Algorithm.h"

#include <gl/gl.h>
#include <gl/glut.h>

class TR_InsertingDiagonals : public Algorithm
{
	struct SmartEdge
	{
		Line l;
		SmartEdge* prev;
		SmartEdge* next;
		SmartEdge(){}
		SmartEdge(Line _l)
		{
			l = _l;
			next = prev = NULL;
		}
		~SmartEdge()
		{
			cout<<"koko"<<endl;
			delete prev, next;
		}
	};
	void reverseSmartPoint(SmartEdge* polygon, int n)
	{
		SmartEdge* current = polygon;
		for(int i = 0; i < n; i ++, current=current->next)
			swap(current->prev, current->next);
	}
	void TransferToCounterClockWise(SmartEdge* polygon, int n)
	{
		SmartEdge* mn = polygon;
		SmartEdge* j = polygon;
		Point mnP(1000000, 1000000);
		for(int i = 0; i < n; i ++, j = j->next)
			if(mnP > j->l.start)
				mnP = j->l.start,
				mn = j;

		Point a, b;
		a = mn->prev->l.start - mn->l.start;
		b = mn->l.start - mn->next->l.start;
		//if left -> counterclockwise
		if(Utilities::cross2Vectors(a,b) > 0)
			return;
		//reverse the list..
		reverseSmartPoint(polygon, n);
	}
	bool isConvex(Point prev, Point p, Point next)
	{
		return Utilities::cross2Vectors(p - prev, next - p) > 0;
	}
	SmartEdge * buildPolygon(vector<Line> inputLines)
	{
		int n = inputLines.size();
		SmartEdge * polygon, *iterator;
		polygon = new SmartEdge(inputLines[0]);
		polygon->l.lineDrawID = 1;
		iterator = polygon;
		for(unsigned i = 1; i < n; i ++)
		{
			iterator->next = new SmartEdge(inputLines[i]);	
			iterator->next->l.lineDrawID = i + 1;
			iterator->next->prev = iterator;
			iterator = iterator->next;
		}
		iterator->next = polygon;
		polygon->prev = iterator;
		return polygon;
	}
	set<SmartEdge*> getAllConvexVertex(int n, SmartEdge* polygon)
	{
		SmartEdge* j = polygon;
		set<SmartEdge*> convexes;
		for(int i = 0; i < n; i ++,j=j->next)
			if(isConvex(j->prev->l.start, j->l.start, j->next->l.start))
				convexes.insert(j);
		return convexes;
	}
	void drawLine(Line l)
	{
		glBegin(GL_LINES);
		glColor3f(1,1,0);
		glLineWidth(5.0);
		glVertex2f(l.start.x, l.start.y);
		glVertex2f(l.end.x, l.end.y);
		glEnd();
		glFlush();
		cout<<l.start.pointDrawID<<" "<<l.end.pointDrawID<<endl;
	}
	bool insertDiagonal(SmartEdge* polygon, set<SmartEdge*>& convexes, Line& outDiagonal)
	{
		SmartEdge* mx;
		float z = 0.0;
		float maxPoint = -1/z;
		bool isInternal = true;
		for(SmartEdge* i = polygon->next->next; i != polygon->prev; i = i->next)
			if(Utilities::PointInTriangle(i->l.start, polygon->next->l.start, polygon->l.start, polygon->prev->l.start))
			{
				z = Utilities::getLinePointDistance(Line(polygon->next->l.start, polygon->prev->l.start), i->l.start);
				if(z > maxPoint)
				{
					maxPoint = z;
					mx = i;
					isInternal = false;
				}
			}
		if(isInternal)
		{
			convexes.erase(polygon);
			if(polygon->next->next->next == polygon)
				return false;
			polygon->prev->next = polygon->next;
			polygon->next->prev = polygon->prev;
			
			convexes.erase(polygon->next);
			convexes.erase(polygon->prev);
			//update neighbours
			Point *before, *point, *after;
			
			before = &polygon->next->prev->l.start;
			point  = &polygon->next->l.start;
			after  = &polygon->next->next->l.start;
			if(isConvex(*before,*point,*after))
				convexes.insert(polygon->next);
			
			before	= &polygon->prev->prev->l.start;
			point	= &polygon->prev->l.start;
			after	= &polygon->prev->next->l.start;
			if(isConvex(*before,*point,*after))
				convexes.insert(polygon->prev);
			outDiagonal = Line(polygon->next->l.start, polygon->prev->l.start);
			return true;
		}
		else
		{
			if(mx->next->next == polygon)
			{
				convexes.erase(mx->next);
				polygon->prev = mx;
				mx->next = polygon;
				outDiagonal = Line(polygon->l.start, mx->l.start);
				return true;
			}
			else if(mx->prev->prev == polygon)
			{
				convexes.erase(mx->prev);
				polygon->next = mx;
				mx->prev = polygon;
				outDiagonal = Line(polygon->l.start, mx->l.start);
				return true;
			}
			else
			{
				Line diagonal(polygon->l.start, mx->l.start);
				Line twin	 (mx->l.start, polygon->l.start);

				SmartEdge *d, *t, *polygonPrev = polygon->prev;
				d = new SmartEdge(diagonal);
				t = new SmartEdge(twin);
				d->l = diagonal, t->l = twin;

				t->next = polygon, t->prev = mx->prev;
				polygon->prev = t, mx->prev->next = t;

				d->next = mx, d->prev = polygonPrev;
				mx->prev = d, polygonPrev->next = d;

				outDiagonal = diagonal;
				return true;
			}
			
		}
	}
public:
	TR_InsertingDiagonals(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		int n = inputLines.size();
		int t;
		set<Line> diagonals;
		SmartEdge* polygon = buildPolygon(inputLines);
		TransferToCounterClockWise(polygon, n);
		//Edges whose start vertex is convex
		set<SmartEdge*> convexes = getAllConvexVertex(n, polygon);
		Line newDiagonal;
		while(convexes.size())
		{
			if(insertDiagonal((*convexes.begin()), convexes, newDiagonal))
			{
				drawLine(newDiagonal);
				outputLines.push_back(newDiagonal);
			}
		}
	}
};
#endif
