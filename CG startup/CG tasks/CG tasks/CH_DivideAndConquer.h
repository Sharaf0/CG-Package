#pragma once
#ifndef CH_DIVIDE_AND_CONQUER
#define CH_DIVIDE_AND_CONQUER
#include "Algorithm.h"
//Todo: check colinear in conditions of building hulls
#define PointAngleIterator set<Point, AngleComparer>::iterator

class CH_DivideAndConquer : public Algorithm
{
	PointAngleIterator next(PointAngleIterator it, const IAnglePointsSet& S)
	{
		it++;
		if(it==S.end())
			it=S.begin();
		return it;
	}
	PointAngleIterator prev(PointAngleIterator it, const IAnglePointsSet& S)
	{
		if(it==S.begin())
			it=S.end();
		it--;
		return it;
	}
	pair<PointAngleIterator, PointAngleIterator>  buildUpperHull(const IAnglePointsSet& A, const IAnglePointsSet& B)
	{
		bool doneBuilding = true;
		PointAngleIterator after, before, iA = A.begin(), iB = B.begin();
		while(!doneBuilding)
		{
			doneBuilding = true;
			//from A to B
			while(1)
			{
				after  = next(iB,B), before = prev(iB,B);
				bool bNotChanged = true;
				if(Utilities::cross2Vectors((*iB)-(*iA),(*before)-(*iB)) < 0)
					iB = before, bNotChanged = false;
				else if(Utilities::cross2Vectors((*iB)-(*iA),(*after)-(*iB)) < 0)
					iB = after, bNotChanged = false;
				if(bNotChanged)
					break;
			}
			//from B to A
			while(1)
			{
				after = next(iA, A), before = prev(iA, A);
				bool aNotChanged = true;
				if(Utilities::cross2Vectors((*iA)-(*iB),(*after)-(*iA))>0)
					iA = after, aNotChanged = false;
				else if(Utilities::cross2Vectors((*iA)-(*iB),(*before)-(*iA))>0)
					iA = before, aNotChanged = false;
				if(aNotChanged)
					break;
			}
			return make_pair(iA,iB);
		}
	}
	pair<PointAngleIterator, PointAngleIterator>  buildLowerHull(const IAnglePointsSet& A, const IAnglePointsSet& B)
	{
		bool doneBuilding = true;
		PointAngleIterator after, before, iA = A.begin(), iB = B.begin();
		while(!doneBuilding)
		{
			doneBuilding = true;
			//from A to B
			while(1)
			{
				after  = next(iB,B), before = prev(iB,B);
				bool bNotChanged = true;
				if(Utilities::cross2Vectors((*iB)-(*iA),(*after)-(*iB)) < 0)
					iB = after, bNotChanged = false;
				else if(Utilities::cross2Vectors((*iB)-(*iA),(*before)-(*iB)) < 0)
					iB = before, bNotChanged = false;
				if(bNotChanged)
					break;
			}
			//from B to A
			while(1)
			{
				after = next(iA, A), before = prev(iA, A);
				bool aNotChanged = true;
				if(Utilities::cross2Vectors((*iA)-(*iB),(*before)-(*iA))>0)				
					iA = before, aNotChanged = false;
				else if(Utilities::cross2Vectors((*iA)-(*iB),(*after)-(*iA))>0)
					iA = after, aNotChanged = false;
				if(aNotChanged)
					break;
			}
			return make_pair(iA,iB);

		}
		IAnglePointsSet merge(IAnglePointsSet A, IAnglePointsSet B)
		{
			pair<PointAngleIterator, PointAngleIterator> tempPair;
			PointAngleIterator aUpper, aLower, bUpper, bLower;

			tempPair = buildUpperHull(A,B);
			aUpper = tempPair.first, bUpper = tempPair.second;

			tempPair = buildLowerHull(A,B);
			aLower = tempPair.first, bLower = tempPair.second;

			while(aUpper != aLower)
				A.erase(aUpper), aUpper = prev(aUpper, A);
			while(bUpper != bLower)
				B.erase(bUpper), bUpper = next(bUpper, B);


		}
		IAnglePointsSet getConvex(IAnglePointsSet S)
		{
			if(S.size()<4)
				return S;
			IAnglePointsSet left, right;
			PointAngleIterator it = S.begin();
			for(int i = 0; i < S.size(); i ++, it++)
			{
				if(i<S.size()/2)
					left.insert((*it));
				else
					right.insert((*it));
			}
			return merge(getConvex(left),getConvex(right));
		}
	public:
		CH_DivideAndConquer(){}

		void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
			vector<Point>& outputPoints, vector<Line>& outputLines)
		{
			vector<Point> input = inputPoints;
			sort(input.begin(), input.end());
			//outputPoints = getConvex(input);

		}
	};
#endif