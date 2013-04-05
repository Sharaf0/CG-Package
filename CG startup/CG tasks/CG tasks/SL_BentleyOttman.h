#pragma once
#ifndef SL_BENTLEYOTTMAN
#define SL_BENTLEYOTTMAN
#include <algorithm>
#include <gl/gl.h>
#include <gl/glut.h>
#include <Windows.h>

#define lineIt set<Line, ActiveLinesComparer>::iterator

enum EVENT_TYPE
{
	LEFT_END_EVENT,
	INTERSECTION_EVENT,
	RIGHT_END_EVENT
};

struct EventPoint
{
public:
	Point currentPoint;
	EVENT_TYPE eventType;//used to sort
	int id;
	static int eventID;
	virtual void handleTransition() = 0;
	virtual Line getCurrentLine() const = 0;
};

EventPoint* currentEvent;

bool isPointLessY(const Point& a, const Point& b)
{
	if(a.y==b.y)
		return a.x > b.x;
	return a.y < b.y;
}

float getCurrentY(const Line& l)
{
	Point p;
	Utilities::computeSegmentIntersection(Point(currentEvent->currentPoint.x, -2000), Point(currentEvent->currentPoint.x, 2000), l.start, l.end, p);
	return p.y;
}

struct EventPointComparer
{
	bool operator()(const EventPoint* lhs, const EventPoint* rhs)const
	{
		if (fabs(lhs->currentPoint.x - rhs->currentPoint.x) >= 1e-3)//if (lhs->currentPoint.x != rhs->currentPoint.x)
			return lhs->currentPoint.x < rhs->currentPoint.x;
		if (fabs(lhs->currentPoint.y - rhs->currentPoint.y) >= 1e-3)//if (lhs->currentPoint.y != rhs->currentPoint.y)
			return lhs->currentPoint.y < rhs->currentPoint.y;
		if(lhs->eventType != rhs->eventType)
			return lhs->eventType < rhs->eventType;
		return lhs->id < rhs->id;
	}
};

struct ActiveLinesComparer
{
	bool operator()(const Line lhs, const Line rhs)const
	{
		if(lhs.lineDrawID == rhs.lineDrawID)
			return false;
		float l = getCurrentY(lhs);
		float r = getCurrentY(rhs);
		if(fabs(l-r)>1e-3)//( l != r)
			return l < r;
		if(lhs.start == rhs.start)
			return isPointLessY(lhs.end, rhs.end);
		if(lhs.end == rhs.end)
			return isPointLessY(lhs.start, rhs.start);
		if(currentEvent->eventType == INTERSECTION_EVENT || lhs.end == rhs.start || lhs.start == rhs.end)
		{
			Line yAxis(currentEvent->currentPoint, Point(currentEvent->currentPoint.x, currentEvent->currentPoint.y+1));
			float lhsAngle = Utilities::getAngle2Vectors(yAxis, Line(currentEvent->currentPoint, lhs.end));
			float rhsAngle = Utilities::getAngle2Vectors(yAxis, Line(currentEvent->currentPoint, rhs.end));
			
			lhsAngle=max(lhsAngle, 0);
			rhsAngle=max(rhsAngle, 0);

			return lhsAngle > rhsAngle;
		}
		//Supposed not to be reached
		return lhs.lineDrawID < rhs.lineDrawID;
		/*
		char* koko=new char[20];
		sprintf(koko, "%s @ %d", eventNames[currentEvent->eventType].c_str(), currentEvent->getCurrentLine().lineDrawID);
		MessageBox(NULL,koko,"Warning",MB_OK);
		delete koko;
		return lhs.lineDrawID < rhs.lineDrawID;
		*/
	}
};

map<pair<int, int>, bool> checkedBefore;
set< Line, ActiveLinesComparer > active;
set<EventPoint*, EventPointComparer> events;
set<Point> intersections;

void drawSweep(float x)
{
	glFlush();
	glBegin(GL_LINES);
	glColor3f(0.5,0.5,1);
	glLineWidth(5.0);
	glVertex2f(x, -1000);
	glVertex2f(x,  1000);
	glEnd();
	glFlush();
}
set<Line, ActiveLinesComparer>::iterator findActiveLine(int id)
{
	set<Line, ActiveLinesComparer>::iterator it = active.begin();
	for(;it!=active.end();it++)
		if(it->lineDrawID == id)
			return it;
	return it;
}
void checkIntersecion(Line, Line, bool);

struct LeftEndPoint : public EventPoint
{
public:
	Line currentLine;
	LeftEndPoint(){}
	LeftEndPoint(Line _l)
	{
		currentLine = _l;
		currentPoint = currentLine.start;
		eventType = LEFT_END_EVENT;
		id = eventID++;
	}
	void handleTransition()
	{
		if(currentLine.isVertical())
		{
			for(lineIt i = active.begin(); i != active.end(); i ++)
				checkIntersecion(currentLine, *i, true);
		}
		else
		{
			lineIt currentIterator = active.insert(currentLine).first;
			
			lineIt before, after;
			before = after = currentIterator;
			before--;
			after ++;
			
			if(after!=active.end())
				checkIntersecion(*after, *currentIterator, false);
			if(before!=active.end())
				checkIntersecion(*before, *currentIterator, false);
		}
	}
	Line getCurrentLine()const
	{
		return currentLine;
	}
};

struct RightEndPoint : public EventPoint
{
	Line currentLine;
	RightEndPoint(){}
	RightEndPoint(Line _l)
	{
		currentLine = _l;
		currentPoint = max(currentLine.start, currentLine.end);
		eventType = RIGHT_END_EVENT;
		id = eventID++;
	}
	void handleTransition()
	{
		if(currentLine.isVertical())
			return;
		lineIt currentIterator = findActiveLine(currentLine.lineDrawID);
		lineIt before, after;
		before = after = currentIterator;
		before--; after++;
		active.erase(currentIterator);
		
		if(after!=active.end() && before!=active.end())
			checkIntersecion(*before, *after, false);
	}
	Line getCurrentLine()const
	{
		return currentLine;
	}
};

struct Intersection : public EventPoint
{
	Line firstLine, secondLine;
	Intersection(){}
	Intersection(Line _l1, Line _l2, Point _p)
	{
		firstLine = _l1, secondLine = _l2, currentPoint = _p;
		eventType = INTERSECTION_EVENT;
		id = eventID++;
	}
	void handleTransition()
	{
		lineIt upper = findActiveLine(firstLine .lineDrawID);
		lineIt lower = findActiveLine(secondLine.lineDrawID);

		active.erase(upper);
		active.erase(lower);

		upper = active.insert(firstLine).first;
		lower = active.insert(secondLine).first;

		lineIt upperOfUpper = upper, lowerOfLower = lower;
		upperOfUpper++, lowerOfLower --;

		if(upper != active.end() && upperOfUpper != active.end())
			checkIntersecion(*upper, *upperOfUpper, false);

		if(lower != active.end() && lowerOfLower != active.end())
			checkIntersecion(*lower, *lowerOfLower, false);
	}
	Line getCurrentLine()const
	{
		return firstLine;
	}
};

void checkIntersecion(Line l1, Line l2, bool isVertical)
{
	if(checkedBefore[make_pair(l1.lineDrawID,l2.lineDrawID)]==true)
		return;
	checkedBefore[make_pair(l1.lineDrawID,l2.lineDrawID)]=true;
	Point intersectionPoint;
	if(Utilities::computeSegmentIntersection(l1.start,l1.end,l2.start,l2.end,intersectionPoint))
	{
		intersections.insert(intersectionPoint).second ;
		if(!isVertical)
			events.insert(new Intersection(l1, l2, intersectionPoint));
	}
}
class SL_BentleyOttman : public Algorithm
{
public:
	SL_BentleyOttman(){}

	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		checkedBefore.clear();
		active.clear();
		events.clear();
		intersections.clear();
		vector<Line> inputSegments = inputLines;
		sort(inputSegments.begin(), inputSegments.end());
		inputSegments.resize(unique(inputSegments.begin(), inputSegments.end())-inputSegments.begin());
		for(int i = 0; i < inputSegments.size(); i ++)
		{
			inputSegments[i].handleOrder();
			events.insert( new LeftEndPoint (inputSegments[i]) );
			events.insert( new RightEndPoint(inputSegments[i]) );
		}
		while(events.size())
		{
			currentEvent = (*events.begin());
			//drawSweep(currentEvent->currentPoint.x);
			events.erase(events.begin());
			currentEvent->handleTransition();
		}

		outputPoints = vector<Point>(intersections.begin(), intersections.end());
	}
};

#endif

/*For Testing Routines

void printActive()
{
	system("cls");
	cout<<eventNames[currentEvent->eventType]<<": "<<currentEvent->getCurrentLine().lineDrawID<<endl;
	cout<<"Active"<<endl;
	for(set<Line,ActiveLinesComparer>::iterator i = active.begin(); i!=active.end();i++)
		cout<<i->lineDrawID<<endl;
}
string eventNames[] = {"LEFT","INTERSECTION","RIGHT"};
void printSet()
{
	cout<<"------------------------------"<<endl;
	for(set<EventPoint*, EventPointComparer>::iterator i = events.begin(); i != events.end(); i ++)
	{
		cout<<"("<<(*i)->currentPoint.x<<","<<(*i)->currentPoint.y<<")"<<endl;
	}
}

*/