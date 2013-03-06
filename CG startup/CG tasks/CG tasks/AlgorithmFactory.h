#pragma once
#ifndef ALGORITHMFACTORY
#define ALGORITHMFACTORY

#include "CH_ExtremePoints.h"
#include "CH_JarvisMarch.h"
#include "CH_ExtremeSegments.h"
#include "CH_QuickHull.h"
#include "CH_GrahamHull.h"
#include "CH_Graham_Hanaa.h"
#include "CH_ExtremePoints_Hanaa.h"
#include"CH_ExtremeSegments_Hanaa.h"
#include "CH_Incremental_Hanaa.h"


using namespace std;

const int ALGORITHMS_NUM = 4;
string algorithmsNames[] = {//"CH_ExtremePoints",
							//"CH_JarvisMarch",
							//"CH_ExtremeSegments",
							//"CH_QuickHull",
							//"CH_GrahamHull",
							"CH_Graham_Hanaa",
							"CH_ExtremePoints_Hanaa",
							"CH_ExtremeSegments_Hanaa",
							"CH_Incremental_Hanaa"};

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
		if(type=="CH_Graham_Hanaa")
			return new CH_Graham_Hanaa();
		if (type=="CH_ExtremePoints_Hanaa")
			return new CH_ExtremePoints_Hanaa();
		if (type == "CH_ExtremeSegments_Hanaa")
			return new CH_ExtremeSegments_Hanaa();
		if (type == "CH_Incremental_Hanaa")
			return new CH_Incremental_Hanaa();

		else
		{
			//std::cerr<<"Can't find such type"<<endl;
			exit(1);
		}
	}
};
#endif