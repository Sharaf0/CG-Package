#pragma once
#ifndef SUBTRACTING_EARS
#define SUBTRACTING_EARS
#include "Algorithm.h"
class TR_SubtractingEars : public Algorithm
{
	struct SmartPoint
	{
		Point p;
		SmartPoint* prev;
		SmartPoint* next;
		SmartPoint(){}
		SmartPoint(Point _p)
		{
			p = _p;
			next = prev = NULL;
		}
		bool operator < (const SmartPoint& rhs)const
		{
			return this->p.pointDrawID < rhs.p.pointDrawID;
		}
		~SmartPoint()
		{
			cout<<"koko"<<endl;
			delete prev, next;
		}
	};
	void printList(SmartPoint* polygon, int n)
	{
		SmartPoint* current = polygon;
		for(int i = 0; i < n; i ++, current=current->next)
			cout<<current->p<<" ";
		cout<<endl;
	}
	void reverseSmartPoint(SmartPoint* polygon, int n)
	{
		SmartPoint* current = polygon;
		for(int i = 0; i < n; i ++, current=current->next)
			swap(current->prev, current->next);
	}
	void TransferToCounterClockWise(SmartPoint* polygon, int n)
	{
		SmartPoint* mn = polygon;
		SmartPoint* j = polygon;
		for(int i = 0; i < n; i ++, j = j->next)
			if(mn->p > j->p)
				mn = j;

		Point a, b;
		a = mn->prev->p - mn->p;
		b = mn->p - mn->next->p;
		//if left -> counterclockwise
		if(Utilities::cross2Vectors(a,b) > 0)
			return;
		//reverse the list..
		reverseSmartPoint(polygon, n);
	}
	bool isConvex(SmartPoint* p)
	{
		return Utilities::cross2Vectors(p->p - p->prev->p, p->next->p - p->p) > 0;
	}
	SmartPoint * buildPolygon(int n, vector<Line> inputLines)
	{
		SmartPoint * polygon, *iterator;
		polygon = new SmartPoint(inputLines[0].start);
		polygon->p.pointDrawID = 1;
		iterator = polygon;
		for(unsigned i = 1; i < n; i ++)
		{
			iterator->next = new SmartPoint(inputLines[i].start);	
			iterator->next->p.pointDrawID = i + 1;
			iterator->next->prev = iterator;
			iterator = iterator->next;
		}
		iterator->next = polygon;
		polygon->prev  = iterator;
		return polygon;
	}
	set<SmartPoint*> getAllEars(int n, SmartPoint* polygon)
	{
		SmartPoint* j = polygon;
		set<SmartPoint*> ears;
		for(int i = 0; i < n; i ++,j=j->next)
			if(isConvex(j))
				if(isEar(j))
					ears.insert(j);
		return ears;
	}
	bool isEar(SmartPoint* point)
	{
		for(SmartPoint* i = point->next->next; i != point->prev; i = i->next)
			if(Utilities::PointInTriangle(i->p, point->next->p, point->p, point->prev->p))
				return false;
		return true;
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
	bool insertDiagonal(SmartPoint* polygon, set<SmartPoint*>& ears, Line& outDiagonal)
	{
		ears.erase(polygon);//subtract ear
		if(polygon->next->next->next == polygon)
				return false;
		polygon->prev->next = polygon->next;
		polygon->next->prev = polygon->prev;

		ears.erase(polygon->next);
		ears.erase(polygon->prev);

		//update neighbours
		if(isConvex(polygon->next))
			if(isEar(polygon->next))
				ears.insert(polygon->next);
		if(isConvex(polygon->prev))
			if(isEar(polygon->prev))
				ears.insert(polygon->prev);
		outDiagonal = Line(polygon->next->p, polygon->prev->p);
		return true;
	}
public:
	TR_SubtractingEars(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		int n = inputLines.size();
		SmartPoint* polygon = buildPolygon(n, inputLines);
		TransferToCounterClockWise(polygon, n);
		set<SmartPoint*> ears = getAllEars(n, polygon);
		Line l;
		while(ears.size())
		{
			if(insertDiagonal((*ears.begin()), ears,l))
			{
				drawLine(l);
				outputLines.push_back(l);
			}
		}
	}
};
#endif
