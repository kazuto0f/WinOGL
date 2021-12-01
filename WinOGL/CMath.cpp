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

double CMath::calcNaiseki(CVect A, CVect B)
{
	return (A.GetXVec() * B.GetXVec() + A.GetYVec() * B.GetYVec());
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
CVect CMath::CalcVect(double sx, double sy, double ex, double ey)
{
	return CVect(ex - sx, ey - sy);
}

//外積の計算
double CMath::calcGaiseki(CVect v1, CVect v2)
{
	return (v1.GetXVec()*v2.GetYVec() - v1.GetYVec()*v2.GetXVec());
}


// //角度の計算{A ~ B}(始点、終点)
double CMath::calcAngle(CVertex* As, CVertex* Ae, CVertex* Bs, CVertex* Be)
{
	CVect A = CalcVect(As, Ae);
	CVect B = CalcVect(Bs, Be);
	return atan2(calcGaiseki(A, B), calcNaiseki(A, B));
}

//点と直線の距離計算(絶対値)
double CMath::calcDistancePL(CVect a, CVect b)
{
	double d = abs(calcGaiseki(a, b)) / sqrt(a.GetXVec() * a.GetXVec() + a.GetYVec() * a.GetYVec());
	return d;
}




//座標をworld座標系に変換
double CMath::ChangeWorld(double width, double height, double devi)
{
	devi = (devi - 0.5) * 2;

	if (width > height) {
		devi = devi * (width / height);
	}
	else {
		devi = devi * (width / height);
	}
	return devi;
}
