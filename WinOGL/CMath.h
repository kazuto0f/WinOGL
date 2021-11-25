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
	double calcNaiseki(CVect A, CVect B);
	//ベクトルの計算
	CVect CalcVect(CVertex, CVertex);
	CVect CalcVect(CVertex*, CVertex*);
	CVect CalcVect(double, double, double, double);

	//外積の計算
	double calcGaiseki(CVect, CVect);
	
	//角度の計算
	double calcAngle(CVertex* As, CVertex* Ae, CVertex* Bs, CVertex* Be);

	//点と直線の距離計算
	double calcDistancePL(CVect a, CVect b);

	//座標をworld座標系に変換
	double ChangeWorld(double width, double height, double devi);
};

