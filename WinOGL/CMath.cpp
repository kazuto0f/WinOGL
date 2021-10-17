#include "pch.h"
#include "CMath.h"
#include <math.h>

double CMath::euclid2p(double p1_x, double p1_y, double p2_x, double p2_y) {
	return sqrt((p1_x - p2_x) * (p1_x - p2_x) + (p1_y - p2_y) * (p1_y - p2_y));
}

double CMath::calcNaiseki(CVertex v1, CVertex v2)
{
	return (v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY());
}

//ベクトルの計算(始点、終点)
CVect CMath::CalcVect(CVertex v1, CVertex v2)
{
	return CVect(v2.GetX() - v1.GetX(), v2.GetY() - v1.GetY());
}
CVect CMath::CalcVect(CVertex* v1, CVertex* v2)
{
	return CVect(v2->GetX() - v1->GetX(), v2->GetY() - v1->GetY());
}

//外積の計算
double CMath::calcGaiseki(CVect v1, CVect v2)
{
	return (v1.GetXVec()*v2.GetYVec() - v1.GetYVec()*v2.GetXVec());
}


