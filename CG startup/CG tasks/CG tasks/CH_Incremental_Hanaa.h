#pragma once
#ifndef CH_INCREMENTAL
#define CH_INCREMENTAL

#include "Algorithm.h"


class CH_Incremental_Hanaa : public Algorithm
{
private:
	

	bool isExterior (vector <Point> & Polygon ,const Point & point)
	{
		bool left= false, right =false;
		for (int i = 0 ; i < Polygon.size()-1 ; i++)
		{
			float product = Utilities::crossProduct(Polygon[i],Polygon[i+1],point);
			if (product >0 ) left = true;
			else if (product<0) right = true;
		}

		float product = Utilities::crossProduct(Polygon[Polygon.size()-1],Polygon[0],point);
		if (product >0 ) left = true;
		else if (product<0) right = true;

		if (left&&right) return true;
		return false;
	}
	void AddNewExteriorPoint (vector <Point> & Polygon ,const Point & point)
	{
		vector <Point> newTriangle ;
		newTriangle.push_back(point);
		bool added = false;

		for (int i = 0 ;i< Polygon.size(); i++)
		{
			if (IsSupportingLine(Polygon,Line(point,Polygon[i])))
			{
				newTriangle.push_back(Polygon[i]);
			}
			if (newTriangle.size()>=3)
				break;
		}
		for (int i = 0 ; i < Polygon.size() ; i++)
		{
			if (i <Polygon.size())
			{
				if ((Polygon[i] == newTriangle[1] && Polygon[(i+1)%Polygon.size()] == newTriangle[2])||
					(Polygon[i] == newTriangle[2] && Polygon[(i+1)%Polygon.size()] == newTriangle[1]))
				{
					Polygon.insert(Polygon.begin()+i+1,point);
					i++;
					added=true;
					continue;
				}
			}
			if (!isExterior(newTriangle, Polygon[i])&& newTriangle[1]!= Polygon[i] && newTriangle[2]!=Polygon[i] )
			{	
				
				Polygon.erase(Polygon.begin()+i);
				if (!added)
				{
					Polygon.insert(Polygon.begin()+i,point);
					added= true;
				}
				else
					i--;
			}
		}
	}
	bool IsSupportingLine (vector <Point> & Polygon ,const Line & line)
	{
		bool left=false,right=false;

		for (int i = 0 ; i <Polygon.size();i++)
		{
			if (!(line.end == Polygon[i]))
			{
				float product = Utilities::crossProduct(line.start,line.end,Polygon[i]);
				if (product >0 ) left = true;
				else right = true;
			}
		}
		if (right && left)
			return false;
		return true;

	}

public:
	CH_Incremental_Hanaa(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{

		outputPoints.clear(), outputLines.clear();
		if (inputPoints.size()<3) return;

		for (int i = 0 ; i < 3 ;i++)
		outputPoints.push_back(inputPoints[i]);

		for (int k = 3 ; k < inputPoints.size() ; k++)
		{
			if (isExterior(outputPoints,inputPoints[k]))
			{
				AddNewExteriorPoint(outputPoints,inputPoints[k]);
			}
		}
		unique(outputPoints.begin(),outputPoints.end());

		for (int i = 0 ; i < outputPoints.size()-1; i++)
		{
			outputLines.push_back(Line(outputPoints[i],outputPoints[i+1]));
		}
		outputLines.push_back(Line(outputPoints[outputPoints.size()-1],outputPoints[0]));
	}
};
#endif