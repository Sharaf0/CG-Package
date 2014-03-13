#pragma once
#ifndef CH_DIVIDE_AND_CONQUER
#define CH_DIVIDE_AND_CONQUER
#include "Algorithm.h"
//Todo: check colinear in conditions of building hulls
#define PointAngleIterator set<Point, AngleComparer>::iterator
#define PointlistIterator list<Point>::iterator;

class CH_DivideAndConquer : public Algorithm
{
	list<Point>::iterator next(list<Point>::iterator it, list<Point>& l)
	{
		it++;
		if(it==l.end())
			it=l.begin();
		return it;
	}
	list<Point>::iterator prev(list<Point>::iterator it, list<Point>& l)
	{
		if(it==l.begin())
			it=l.end();
		it--;
		return it;
	}
	pair<list<Point>::iterator, list<Point>::iterator>  buildUpperHull(list<Point>& A, list<Point>& B)
	{
		bool doneBuilding = false;
		list<Point>::iterator after, before, iA = max_element(A.begin(),A.end()), iB = min_element(B.begin(), B.end());
		while(!doneBuilding)
		{
			doneBuilding = true;
			//from A to B
			while(1)
			{
				after  = next(iB,B), before = prev(iB,B);
				//if(after==before)
				//	break;
				bool bNotChanged = true;
				if(Utilities::cross2Vectors((*iB)-(*iA),(*before)-(*iB)) > 0)
					iB = before, bNotChanged = doneBuilding = false;
				else if(Utilities::cross2Vectors((*iB)-(*iA),(*after)-(*iB)) > 0)
					iB = after, bNotChanged = doneBuilding = false;
				else if(Utilities::cross2Vectors((*iB)-(*iA),(*before)-(*iB)) == 0)
				{
					if(Utilities::pointOnSegment((*iA), (*before), (*iB)))
						iB = before, bNotChanged = doneBuilding = false;
				}
				if(bNotChanged)
					break;
			}
			//from B to A
			while(1)
			{
				after = next(iA, A), before = prev(iA, A);
				//if(after==before)
				//	break;
				bool aNotChanged = true;
				if(Utilities::cross2Vectors((*iA)-(*iB),(*after)-(*iA))<0)
					iA = after, aNotChanged = doneBuilding = false;
				else if(Utilities::cross2Vectors((*iA)-(*iB),(*before)-(*iA))<0)
					iA = before, aNotChanged = doneBuilding = false;
				else if(Utilities::cross2Vectors((*iA)-(*iB),(*after)-(*iA))==0)
				{
					if(Utilities::pointOnSegment((*iB), (*after), (*iA)))
						iA = after, aNotChanged = doneBuilding = false;
				}
				if(aNotChanged)
					break;
			}
		}
		return make_pair(iA,iB);
	}
	pair<list<Point>::iterator, list<Point>::iterator>  buildLowerHull(list<Point>& A, list<Point>& B)
	{
		bool doneBuilding = false;
		list<Point>::iterator after, before, iA = max_element(A.begin(),A.end()), iB = min_element(B.begin(), B.end());
		while(!doneBuilding)
		{
			doneBuilding = true;
			//from A to B
			while(1)
			{
				after  = next(iB,B), before = prev(iB,B);
				//if(after==before)
				//	break;
				bool bNotChanged = true;
				if(Utilities::cross2Vectors((*iB)-(*iA),(*after)-(*iB)) < 0)
					iB = after, bNotChanged = doneBuilding = false;
				else if(Utilities::cross2Vectors((*iB)-(*iA),(*before)-(*iB)) < 0)
					iB = before, bNotChanged = doneBuilding = false;
				else if(Utilities::cross2Vectors((*iB)-(*iA),(*after)-(*iB)) == 0)
				{
					if(Utilities::pointOnSegment((*iA), (*after), (*iB)))
						iB = after, bNotChanged = doneBuilding = false;
				}
				if(bNotChanged)
					break;
			}
			//from B to A
			while(1)
			{
				after = next(iA, A), before = prev(iA, A);
				//if(after==before)
				//	break;
				bool aNotChanged = true;
				if(Utilities::cross2Vectors((*iA)-(*iB),(*before)-(*iA))>0)
					iA = before, aNotChanged = doneBuilding = false;
				else if(Utilities::cross2Vectors((*iA)-(*iB),(*after)-(*iA))>0)
					iA = after, aNotChanged = doneBuilding = false;
				else if(Utilities::cross2Vectors((*iA)-(*iB),(*before)-(*iA))==0)
				{
					if(Utilities::pointOnSegment((*iB), (*before), (*iA)))
						iA = before, aNotChanged = doneBuilding = false;
				}
				if(aNotChanged)
					break;
			}
		}
		return make_pair(iA,iB);

	}
	list<Point> merge(list<Point> A, list<Point> B)
	{
		pair<list<Point>::iterator, list<Point>::iterator> tempPair;
		list<Point>::iterator aUpper, aLower, bUpper, bLower;

		tempPair = buildUpperHull(A,B);
		aUpper = tempPair.first, bUpper = tempPair.second;

		tempPair = buildLowerHull(A,B);
		aLower = tempPair.first, bLower = tempPair.second;

		list<Point> merged;
		while(bLower!=bUpper)
		{
			merged.push_back((*bLower));
			bLower = next(bLower,B);
		}
		merged.push_back((*bLower));
		while(aUpper!=aLower)
		{
			merged.push_back((*aUpper));
			aUpper = next(aUpper, A);
		}
		merged.push_back((*aUpper));
		return merged;
	}
	list<Point> getConvex(list<Point> S)
	{
		if(S.size()<3)
			return S;
		list<Point> left, right;
		list<Point>::iterator it = S.begin();
		for(int i = 0; i < S.size(); i ++, it=next(it,S))
			if(i<S.size()/2) left.push_back((*it));
			else right.push_back((*it));
		return merge(getConvex(left),getConvex(right));
	}
public:
	CH_DivideAndConquer(){}

	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		vector<Point> inVector = inputPoints;
		sort(inVector.begin(),inVector.end());
		inVector.resize(unique(inVector.begin(),inVector.end())-inVector.begin());

		if(inVector.size()<3)
		{
			if(inVector.size()==2)
			{
				outputPoints.push_back(inVector.front());
				outputPoints.push_back(inVector.back());
				outputLines.push_back(Line(inVector.front(), inVector.back()));
			}
			return;
		}

		list<Point> input;
		for(unsigned i = 0; i < inVector.size(); i ++)
			input.push_back(inVector[i]);
		list<Point> output = getConvex(input);
		for(list<Point>::iterator i = output.begin(); i != output.end(); i ++)
		{
			list<Point>::iterator j = i;
			j ++;
			if(j==output.end())
				j = output.begin();
			outputPoints.push_back((*i));
			outputLines.push_back(Line((*i),(*j)));
		}
	}
};
#endif