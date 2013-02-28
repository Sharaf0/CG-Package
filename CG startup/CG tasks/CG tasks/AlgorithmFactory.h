#pragma once
#ifndef ALGORITHMFACTORY
#define ALGORITHMFACTORY

#include "CH_ExtremePoints.h"
#include "CH_JarvisMarch.h"
#include "CH_ExtremeSegments.h"

using namespace std;

const int ALGORITHMS_NUM = 3;
string algorithmsNames[] = {"CH_ExtremePoints","CH_JarvisMarch", "CH_ExtremeSegments"};

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
		else
		{
			//std::cerr<<"Can't find such type"<<endl;
			exit(1);
		}
	}
};
#endif