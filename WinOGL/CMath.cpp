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




//�x�N�g���̌v�Z(�n�_�A�I�_)
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

//�O�ς̌v�Z
double CMath::calcGaiseki(CVect v1, CVect v2)
{
	return (v1.GetXVec()*v2.GetYVec() - v1.GetYVec()*v2.GetXVec());
}


// //�p�x�̌v�Z{A ~ B}(�n�_�A�I�_)
double CMath::calcAngle(CVertex* As, CVertex* Ae, CVertex* Bs, CVertex* Be)
{
	CVect A = CalcVect(As, Ae);
	CVect B = CalcVect(Bs, Be);
	return atan2(calcGaiseki(A, B), calcNaiseki(A, B));
}

//�_�ƒ����̋����v�Z(��Βl)
double CMath::calcDistancePL(CVect a, CVect b)
{
	double d = abs(calcGaiseki(a, b)) / sqrt(a.GetXVec() * a.GetXVec() + a.GetYVec() * a.GetYVec());
	return d;
}




//���W��world���W�n�ɕϊ�
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
