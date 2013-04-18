#pragma once
#ifndef TR_MONOTONE_PARTITIONING
#define TR_MONOTONE_PARTITIONING
#include "Algorithm.h"

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
enum EventType
{
	START_VERTEX,
	SPLIT_VERTEX,
	MERGE_VERTEX,
	REGULAR_VERTEX,
	END_VERTEX
};
EventType getSuitableEventType(SmartEdge* e)
{
	if(e->prev->l.start.y < e->l.start.y && e->next->l.start.y < e->l.start.y)
	{
		if(Utilities::cross2Vectors(e->l.start - e->prev->l.start, e->next->l.start - e->l.start) > 0)
			return START_VERTEX;
		else
			return SPLIT_VERTEX;
	}
	else if(e->prev->l.start.y > e->l.start.y && e->next->l.start.y > e->l.start.y)
	{
		if(Utilities::cross2Vectors(e->l.start - e->prev->l.start, e->next->l.start - e->l.start) < 0)
			return MERGE_VERTEX;
		else
			return END_VERTEX;
	}
	return REGULAR_VERTEX;
}
class ActiveLineComparer
{
public:
	bool operator() (SmartEdge* lhs, SmartEdge* rhs)
	{
		return lhs->l.start < rhs->l.start;
	}
};
struct Event
{
	SmartEdge* eventLine;
	EventType typeOfEvent;
	virtual void handleTransition(set<SmartEdge*,ActiveLineComparer>& activeLines, map<SmartEdge*, SmartEdge*>& helper, vector<Line>& outputLines) = 0;
};
class EventComparer
{
public:
	bool operator () (const Event* lhs, const Event* rhs) const
	{
		return lhs->eventLine->l.start.y < rhs->eventLine->l.start.y;
	}
};
struct StartVertex : public Event
{
	StartVertex(){}
	StartVertex(SmartEdge* _l)
	{
		eventLine = _l;
		typeOfEvent = START_VERTEX;
	}
	void handleTransition(set<SmartEdge*,ActiveLineComparer>& activeLines, map<SmartEdge*, SmartEdge*>& helper, vector<Line>& outputLines) 
	{
		activeLines.insert(eventLine);
		helper[eventLine] = eventLine;
	}
};
struct EndVertex : public Event
{
	EndVertex(){}
	EndVertex(SmartEdge* _l)
	{
		eventLine = _l;
		typeOfEvent = END_VERTEX;
	}
	void handleTransition(set<SmartEdge*,ActiveLineComparer>& activeLines, map<SmartEdge*, SmartEdge*>& helper, vector<Line>& outputLines)
	{
		if(getSuitableEventType(helper[eventLine->prev]) == MERGE_VERTEX)
			outputLines.push_back(Line(eventLine->l.start, helper[eventLine->prev]->l.start));
		activeLines.erase(eventLine->prev);
	}
};
struct SplitVertex : public Event
{
	SplitVertex(){}
	SplitVertex(SmartEdge* _l)
	{
		eventLine = _l;
		typeOfEvent = SPLIT_VERTEX;
	}
	void handleTransition(set<SmartEdge*,ActiveLineComparer>& activeLines, map<SmartEdge*, SmartEdge*>& helper, vector<Line>& outputLines)
	{
		set<SmartEdge*, ActiveLineComparer>::iterator leftEdgeIterator;
		activeLines.insert(eventLine);
		leftEdgeIterator = activeLines.find(eventLine);
		leftEdgeIterator --;
		SmartEdge* leftEdge = (*leftEdgeIterator);
		activeLines.erase(eventLine);

		outputLines.push_back(Line(eventLine->l.start, helper[leftEdge]->l.start));
		helper[leftEdge] = eventLine;
		activeLines.insert(eventLine);
		helper[eventLine] = eventLine;
	}
};
struct MergeVertex : public Event
{
	MergeVertex(){}
	MergeVertex(SmartEdge* _l)
	{
		eventLine = _l;
		typeOfEvent = MERGE_VERTEX;
	}
	void handleTransition(set<SmartEdge*,ActiveLineComparer>& activeLines, map<SmartEdge*, SmartEdge*>& helper, vector<Line>& outputLines)
	{
		if(getSuitableEventType(helper[eventLine->prev]) == MERGE_VERTEX)
			outputLines.push_back(Line(eventLine->l.start, helper[eventLine->prev]->l.start));
		activeLines.erase(eventLine->prev);

		activeLines.insert(eventLine);
		set<SmartEdge*,ActiveLineComparer>::iterator leftEdgeIterator = activeLines.find(eventLine);
		leftEdgeIterator--;
		SmartEdge* leftEdge = (*leftEdgeIterator);
		activeLines.erase(eventLine);

		if(getSuitableEventType(helper[leftEdge]) == MERGE_VERTEX)
			outputLines.push_back(Line(eventLine->l.start, helper[leftEdge]->l.start));
		helper[leftEdge] = eventLine;
		return;
	}
};
struct RegularVertex : public Event
{
	RegularVertex(){}
	RegularVertex(SmartEdge* _l)
	{
		eventLine = _l;
		typeOfEvent = REGULAR_VERTEX;
	}
	void handleTransition(set<SmartEdge*,ActiveLineComparer>& activeLines, map<SmartEdge*, SmartEdge*>& helper, vector<Line>& outputLines)
	{
		if(eventLine->prev->l.start.y > eventLine->next->l.start.y)
		{
			if(getSuitableEventType(helper[eventLine->prev]) == MERGE_VERTEX)
				outputLines.push_back(Line(eventLine->l.start, helper[eventLine->prev]->l.start));
			activeLines.erase(eventLine->prev);
			activeLines.insert(eventLine);
			helper[eventLine] = eventLine;
		}
		else
		{
			set<SmartEdge*, ActiveLineComparer>::iterator leftEdgeIterator;
			activeLines.insert(eventLine);
			leftEdgeIterator = activeLines.find(eventLine);
			leftEdgeIterator --;
			SmartEdge* leftEdge = (*leftEdgeIterator);
			activeLines.erase(eventLine);

			if(getSuitableEventType(helper[leftEdge])==MERGE_VERTEX)
			{	
				outputLines.push_back(Line(eventLine->l.start, helper[leftEdge]->l.start));
				helper[leftEdge] = eventLine;
			}
		}
	}
};

Event* getSuitableEvent(SmartEdge* e)
{
	EventType type = getSuitableEventType(e);
	if(type == START_VERTEX)
		return new StartVertex(e);
	if(type == SPLIT_VERTEX)
		return new SplitVertex(e);
	if(type == MERGE_VERTEX)
		return new MergeVertex(e);
	if(type == REGULAR_VERTEX)
		return new RegularVertex(e);
	if(type == END_VERTEX)
		return new EndVertex(e);
}
struct koko { float r, g, b; koko(){} koko(float _r, float _g, float _b){r=_r, g=_g, b=_b;}};
void printStatus(Event* currentEvent, set<SmartEdge*, ActiveLineComparer> activeLines, map<SmartEdge*, SmartEdge*> helpers)
{
	system("cls");
	cout <<"Current event point: "<< currentEvent->eventLine->l.lineDrawID << endl;
	cout<<"Active Lines:"<<endl;
	for(set<SmartEdge*, ActiveLineComparer>::iterator it = activeLines.begin(); it != activeLines.end(); it++)
		cout<<(*it)->l.lineDrawID<<" -> "<<helpers[(*it)]->l.lineDrawID <<endl;
}
class TR_MonotonePartitioning : public Algorithm
{
public:
	TR_MonotonePartitioning(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{//					start = sky, split = red, merge = white, reg = blue , end = yellow
		koko colors[] = { koko(0,1,1), koko(1,0,0), koko(1,1,1), koko(0.5,0.5,1), koko(1,1,0) };
		int n = inputLines.size();
		SmartEdge* polygon = buildPolygon(inputLines);
		TransferToCounterClockWise(polygon, n);
		priority_queue<Event*, vector<Event*>, EventComparer> events;
		SmartEdge* iterator = polygon;
		for(int i = 0; i < n; i ++, iterator = iterator->next)
		{
			Event* current;
			current = getSuitableEvent(iterator);
			glColor3f(colors[current->typeOfEvent].r,colors[current->typeOfEvent].g,colors[current->typeOfEvent].b);
			glPointSize(10);
			glBegin(GL_POINTS);
				glVertex2f(iterator->l.start.x, iterator->l.start.y);
			glEnd();
			glFlush();
			events.push(current);
		}
		set<SmartEdge*, ActiveLineComparer> activeLines;
		map<SmartEdge*, SmartEdge*> helpers;
		while(events.size())
		{
			events.top()->handleTransition(activeLines, helpers, outputLines);
			//printStatus(events.top(), activeLines, helpers);
			events.pop();
		}
	}
};

#endif