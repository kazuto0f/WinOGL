#pragma once
#include "CVertex.h"
#include "CVect.h"
class CMath
{

public:
	//2�_�Ԃ̃��[�N���b�h����
	double euclid2p(double, double, double, double);
	//�x�N�g���̓���
	double calcNaiseki(CVertex, CVertex);
	double calcNaiseki(CVect A, CVect B);
	//�x�N�g���̌v�Z
	CVect CalcVect(CVertex, CVertex);
	CVect CalcVect(CVertex*, CVertex*);
	CVect CalcVect(double, double, double, double);

	//�O�ς̌v�Z
	double calcGaiseki(CVect, CVect);
	
	// //�p�x�̌v�Z
	double calcAngle(CVertex* As, CVertex* Ae, CVertex* Bs, CVertex* Be);
};

