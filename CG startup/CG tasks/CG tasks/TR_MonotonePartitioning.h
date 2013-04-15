#pragma once
#ifndef TR_MONOTONE_PARTITIONING
#define TR_MONOTONE_PARTITIONING
#include "Algorithm.h"

//struct SmartEdge
//{
//	Line l;
//	SmartEdge* prev;
//	SmartEdge* next;
//	SmartEdge(){}
//	SmartEdge(Line _l)
//	{
//		l = _l;
//		next = prev = NULL;
//	}
//};
//void reverseSmartPoint(SmartEdge* polygon, int n)
//{
//	SmartEdge* current = polygon;
//	for(int i = 0; i < n; i ++, current=current->next)
//		swap(current->prev, current->next);
//}
//void TransferToCounterClockWise(SmartEdge* polygon, int n)
//{
//	SmartEdge* mn = polygon;
//	SmartEdge* j = polygon;
//	Point mnP(1000000, 1000000);
//	for(int i = 0; i < n; i ++, j = j->next)
//		if(mnP > j->l.start)
//			mnP = j->l.start,
//			mn = j;
//
//	Point a, b;
//	a = mn->prev->l.start - mn->l.start;
//	b = mn->l.start - mn->next->l.start;
//	//if left -> counterclockwise
//	if(Utilities::cross2Vectors(a,b) > 0)
//		return;
//	//reverse the list..
//	reverseSmartPoint(polygon, n);
//}
//SmartEdge * buildPolygon(vector<Line> inputLines)
//{
//	int n = inputLines.size();
//	SmartEdge * polygon, *iterator;
//	polygon = new SmartEdge(inputLines[0]);
//	polygon->l.lineDrawID = 1;
//	iterator = polygon;
//	for(unsigned i = 1; i < n; i ++)
//	{
//		iterator->next = new SmartEdge(inputLines[i]);	
//		iterator->next->l.lineDrawID = i + 1;
//		iterator->next->prev = iterator;
//		iterator = iterator->next;
//	}
//	iterator->next = polygon;
//	polygon->prev = iterator;
//	return polygon;
//}
//enum EventType
//{
//	START_VERTEX,
//	SPLIT_VERTEX,
//	MERGE_VERTEX,
//	REGULAR_VERTEX,
//	END_VERTEX
//};
//struct Helper
//{
//	Point p;
//	EventType typeOfEvent;
//};
//struct Event
//{
//	SmartEdge* eventLine;
//	EventType typeOfEvent;
//	virtual void handleTransition(set<Line> activeLines, map<Line, Point> helper) = 0;
//};
//
//struct StartVertex : public Event
//{
//	StartVertex(){}
//	StartVertex(SmartEdge* _l)
//	{
//		eventLine = _l;
//	}
//	void handleTransition(set<Line> activeLines, map<SmartEdge*, Point> helper) 
//	{
//		activeLines.insert(eventLine);
//		helper[eventLine] = eventLine->l.start;
//	}
//};
//struct EndVertex : public Event
//{
//	EndVertex(){}
//	EndVertex(SmartEdge* _l)
//	{
//		eventLine = _l;
//	}
//	void handleTransition(set<Line> activeLines, map<Line, Point> helper) 
//	{
///*		if h(e    ) is a merge vertex
//			then insert the diagonal
//                     connecting v   to h(e    ) 
//                     in D
//*/
//		if(helper\
//		activeLines.erase(eventLine->prev);
//
//	}
//};
/*
struct SplitVertex : public Event
{
	SplitVertex(){}
	SplitVertex(Line _l)
	{
		eventLine = _l;
	}
	void handleTransition(set<Line> activeLines, map<Line, Point> helper) 
	{
	}
};
struct MergeVertex : public Event
{
	MergeVertex(){}
	MergeVertex(Line _l)
	{
		eventLine = _l;
	}
	void handleTransition(set<Line> activeLines, map<Line, Point> helper) 
	{
	}
};
struct RegularVertex : public Event
{
	RegularVertex(){}
	RegularVertex(Line _l)
	{
		eventLine = _l;
	}
	void handleTransition(set<Line> activeLines, map<Line, Point> helper) 
	{
	}
};
*/
class TR_MonotonePartitioning : public Algorithm
{
public:
	TR_MonotonePartitioning(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
//		int n = inputLines.size();
//		SmartEdge* polygon = buildPolygon(inputLines);
//		TransferToCounterClockWise(polygon, n);
//		priority_queue<Event*> events;
//		set<Line> activeLines;
//		map<SmartEdge*, Point> helper;
//		while(events.size())
//		{
////			events.top()->handleTransition(activeLines, helper);
//			events.pop();
//		}
	}
};

#endif