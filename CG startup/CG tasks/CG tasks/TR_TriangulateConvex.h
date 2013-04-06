#pragma once
#ifndef CONVEX_TRIANGULATION
#define CONVEX_TRIANGULATION
#include "Algorithm.h"
#include <gl/gl.h>
#include <gl/glut.h>

class TR_ConvexTriangulation : public Algorithm
{
public:
	TR_ConvexTriangulation(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		int n = inputLines.size();
		int r = (rand()%n+1)%n;
		for(unsigned i = 0; i < inputLines.size(); i ++)
		{
			if(i==r||i==r-1||i==(r+1)%n)
				continue;
			outputLines.push_back(Line(inputLines[r].start, inputLines[i].start));
		}
	}
};
#endif