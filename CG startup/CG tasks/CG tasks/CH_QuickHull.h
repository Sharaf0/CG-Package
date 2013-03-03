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
			if(Utilities::crossProduct(S[i],v1.start,v1.end)>0)
				set1.push_back(S[i]);
			if(Utilities::crossProduct(S[i],v2.start,v2.end)>0)
				set2.push_back(S[i]);
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
		if(inputPoints.size()<3)return;
		outputPoints.clear(), outputLines.clear();
		
		int minPointIndex, maxPointIndex;
		Point minPoint( 10000, 10000);
		Point maxPoint(-10000,-10000);

		for(unsigned i = 0; i < inputPoints.size(); i ++)
		{
			if(inputPoints[i]<minPoint)
				minPoint = inputPoints[i],
				minPointIndex = i;

			if(maxPoint<inputPoints[i])
				maxPoint = inputPoints[i],
				maxPointIndex = i;
		}
		vector<Point> upPoints;
		vector<Point> downPoints;

		for(unsigned i = 0; i < inputPoints.size(); i ++)
		{
			float cross = Utilities::crossProduct(inputPoints[i], minPoint,maxPoint);
			if(cross>0)//left
				upPoints.push_back(inputPoints[i]);
			if(cross<0)//right
				downPoints.push_back(inputPoints[i]);
		}
		vector<Point> upHull, downHull;
		upHull   = quickHull(minPoint,maxPoint,upPoints);
		downHull = quickHull(maxPoint,minPoint,downPoints);
		outputPoints.insert(outputPoints.end(),upHull.begin(),upHull.end());
		outputPoints.insert(outputPoints.end(),downHull.begin(),downHull.end());
		outputPoints.resize(unique(outputPoints.begin(),outputPoints.end())-outputPoints.begin());
		for(unsigned i = 0; i < outputPoints.size()-1; i ++)
			outputLines.push_back(Line(outputPoints[i],outputPoints[i+1]));
	}
};
#endif