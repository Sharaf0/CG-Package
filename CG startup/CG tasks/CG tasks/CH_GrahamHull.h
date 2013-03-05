#pragma once
#ifndef CH_GRAHAMHULL
#define CH_GRAHAMHULL
#include "Algorithm.h"

class CH_GrahamHull : public Algorithm
{
	void sortAntiClockWise(vector<Point>& pnts)
	{
		double zero = 0.0;
		Point mn(1 / zero, 1 / zero);//OO
		for (int i = 0; i < pnts.size(); i++)
			if (make_pair(pnts[i].y, pnts[i].x) < make_pair(mn.y, mn.x))
				mn = pnts[i];
		sort(pnts.begin(),pnts.end(),AngleComparer(mn));
	}

public:
	CH_GrahamHull(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		outputPoints.clear(), outputLines.clear();

		vector<Point> input = inputPoints;
		sortAntiClockWise(input);
		input.resize(unique(input.begin(),input.end())-input.begin());
		if(input.size()<3)return;
		outputPoints.push_back(input[0]);
		outputPoints.push_back(input[1]);
		for (int i = 2; i <= input.size(); i++)
		{
			Point c = input[i % input.size()];
			while (outputPoints.size() > 1)
			{
				Point b = outputPoints.back();
				Point a = outputPoints[outputPoints.size()- 2];
				if (Utilities::crossProduct(b,a,c) < -EPS)
					break;
				outputPoints.pop_back();
			}
			if (i < input.size())
				outputPoints.push_back(input[i]);
		}
		for(unsigned i = 0; i < outputPoints.size(); i ++)
			outputLines.push_back(Line(outputPoints[i],outputPoints[(i+1)%outputPoints.size()]));
		if(outputPoints.size() < 3)
			outputPoints.clear(), outputLines.clear();
	}
};
#endif