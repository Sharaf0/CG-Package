#pragma once

#include "Algorithm.h"
#include "CH_JarvisMarch.h"
#include "CH_ExtremePoints.h"

#include <string>
using namespace std;

const int ALGORITHMS_NUM = 2;
string algorithmsNames[] = {"CH_ExtremePoints","CH_JarvisMarch"};

class AlgorithmFactory
{
public:
	Algorithm* createAlgorithm(const string& type)
	{
		if(type=="CH_ExtremePoints")
			return new CH_ExtremePoints();
		if(type=="CH_JarvisMarch")
			return new CH_JarvisMarch();

	}
};