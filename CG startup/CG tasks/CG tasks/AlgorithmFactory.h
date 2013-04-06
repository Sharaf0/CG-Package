#pragma once
#ifndef ALGORITHMFACTORY
#define ALGORITHMFACTORY

//Convex Hull
#include "CH_ExtremePoints.h"
#include "CH_JarvisMarch.h"
#include "CH_ExtremeSegments.h"
#include "CH_QuickHull.h"
#include "CH_GrahamHull.h"
#include "CH_Incremental.h"
#include "CH_DivideAndConquer.h"
//Sweep Line
#include "SL_BentleyOttman.h"
#include "SL_BruteForce.h"
//Triangulation
#include "TR_InsertingDiagonals.h"
#include "TR_SubtractingEars.h"
#include "TR_TriangulateConvex.h"
#include "TR_TriangulateMonotone.h"

using namespace std;

const int ALGORITHMS_NUM = 13;
string algorithmsNames[] = {"CH_ExtremePoints",
							"CH_JarvisMarch",
							"CH_ExtremeSegments",
							"CH_QuickHull",
							"CH_GrahamHull",
							"CH_Incremental",
							"CH_DivideAndConquer",
							"SL_BentleyOttman",
							"SL_BruteForce",
							"TR_InsertingDiagonals",
							"TR_SubtractingEars",
							"TR_TriangulateConvex",
							"TR_TriangulateMonotone"
							};

class AlgorithmFactory
{
public:
	static Algorithm* createAlgorithm(const string& type)
	{
		if(type=="CH_ExtremePoints")
			return new CH_ExtremePoints();
		if(type=="CH_JarvisMarch")
			return new CH_JarvisMarch();
		if(type=="CH_ExtremeSegments")
			return new CH_ExtremeSegments();
		if(type=="CH_QuickHull")
			return new CH_QuickHull();
		if(type=="CH_GrahamHull")
			return new CH_GrahamHull();
		if(type=="CH_Incremental")
			return new CH_Incremental();
		if(type=="CH_DivideAndConquer")
			return new CH_DivideAndConquer();
		if(type=="SL_BentleyOttman")
			return new SL_BentleyOttman();
		if(type=="SL_BruteForce")
			return new SL_BruteForce();
		if(type=="TR_InsertingDiagonals")
			return new TR_InsertingDiagonals();
		if(type=="TR_SubtractingEars")
			return new TR_SubtractingEars();
		if(type=="TR_TriangulateConvex")
			return new TR_ConvexTriangulation();
		if(type=="TR_TriangulateMonotone")
			return new TR_MonotoneTriangulation();

		else
		{
			//std::cerr<<"Can't find such type"<<endl;
			exit(1);
		}
	}
};
#endif