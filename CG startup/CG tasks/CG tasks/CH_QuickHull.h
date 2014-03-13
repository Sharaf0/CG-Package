#pragma once
#ifndef CH_QUICKHULL
#define CH_QUICKHULL

#include "Algorithm.h"

class CH_QuickHull : public Algorithm
{
private:
	vector<Point>quickHull(Point minPoint, Point maxPoint, vector<Point> S)
	{
		if(S.size()==0)
			return vector<Point>();

		Line origin(minPoint, maxPoint);
		Point farthestPoint;
		float maxDistance = -10000, temp;

		for(unsigned i = 0; i < S.size(); i ++)
		{
			temp = Utilities::getLinePointDistance(origin,S[i]);
			if(temp>maxDistance)
				maxDistance=temp,
				farthestPoint = S[i];
		}

		vector<Point> set1, set2;

		Line v1(minPoint,farthestPoint);
		Line v2(farthestPoint,maxPoint);

		for(unsigned i = 0; i < S.size(); i ++)
		{
			float cross1 = Utilities::cross2Vectors(v1.end - v1.start, S[i] - v1.end);
			float cross2 = Utilities::cross2Vectors(v2.end - v2.start, S[i] - v2.end);

			if(cross1 > 0)
				set1.push_back(S[i]);
			if(cross2 > 0)
				set2.push_back(S[i]);

			/*if(Utilities::crossProduct(S[i],v1.start,v1.end)>0)
				set1.push_back(S[i]);
			if(Utilities::crossProduct(S[i],v2.start,v2.end)>0)
				set2.push_back(S[i]);*/
		}
		vector<Point> output;

		vector<Point> leftExtreme  = quickHull(v1.start,v1.end,set1);
		vector<Point> rightExtreme = quickHull(v2.start,v2.end,set2);

		output.push_back(minPoint);
		output.insert(output.end(),leftExtreme.begin(),leftExtreme.end());
		output.push_back(farthestPoint);
		output.insert(output.end(),rightExtreme.begin(),rightExtreme.end());
		output.push_back(maxPoint);

		return output;
	}
public:
	CH_QuickHull(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		vector<Point> input = inputPoints;
		sort(input.begin(),input.end());
		input.resize(unique(input.begin(),input.end())-input.begin());

		if(input.size()<3)
		{
			if(input.size()==2)
			{
				outputPoints.push_back(input.front());
				outputPoints.push_back(input.back());
				outputLines.push_back(Line(input.front(), input.back()));
			}
			return;
		}

		int minPointIndex, maxPointIndex;
		Point minPoint( 10000, 10000);
		Point maxPoint(-10000,-10000);

		for(unsigned i = 0; i < input.size(); i ++)
		{
			if(input[i]<minPoint)
				minPoint = input[i],
				minPointIndex = i;

			if(maxPoint<input[i])
				maxPoint = input[i],
				maxPointIndex = i;
		}
		vector<Point> upPoints;
		vector<Point> downPoints;

		for(unsigned i = 0; i < input.size(); i ++)
		{
			float cross = Utilities::cross2Vectors(maxPoint - minPoint, input[i] - maxPoint);
			if(cross > 0)//left
				upPoints.push_back(input[i]);
			if(cross < 0)//right
				downPoints.push_back(input[i]);
		}
		vector<Point> upHull, downHull;
		upHull   = quickHull(minPoint,maxPoint,upPoints);
		downHull = quickHull(maxPoint,minPoint,downPoints);
		outputPoints.insert(outputPoints.end(),upHull.begin(),upHull.end());
		outputPoints.insert(outputPoints.end(),downHull.begin(),downHull.end());
		outputPoints.resize(unique(outputPoints.begin(),outputPoints.end())-outputPoints.begin());
		if((!upHull.size())||(!downHull.size()))
			outputLines.push_back(Line(minPoint, maxPoint)),
			outputPoints.push_back(minPoint),
			outputPoints.push_back(maxPoint);
		if(outputPoints.size()<2)
			return;
		for(unsigned i = 0; i < outputPoints.size()-1; i ++)
			outputLines.push_back(Line(outputPoints[i],outputPoints[i+1]));
	}
};
#endif