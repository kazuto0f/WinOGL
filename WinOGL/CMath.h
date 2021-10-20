#pragma once
#include "CVertex.h"
#include "CVect.h"
class CMath
{

public:
	//2点間のユークリッド距離
	double euclid2p(double, double, double, double);
	//ベクトルの内積
	double calcNaiseki(CVertex, CVertex);
	//ベクトルの計算
	CVect CalcVect(CVertex, CVertex);
	CVect CalcVect(CVertex*, CVertex*);
	CVect CalcVect(double, double, double, double);

	//外積の計算
	double calcGaiseki(CVect, CVect);
	
};

