#pragma once
#include "CVertex.h"
#include "CShape.h"
#include "CMath.h"


class CAdminControl
{
public:
	CAdminControl();
	~CAdminControl();

	//OnDraw�֐�
	void OnDraw();

	//�N���b�N���W�̕ۑ�
	void OnClick(double,double,int,int);

	//Shape�̉��
	void FreeShape();

	//Vertex�����߂̓_�ƈʒu���邩
	void CheckClosed(CShape* head);
	
	//��������
	bool CheckCrossShape(CShape* head);

private:
	double World_X;
	double World_Y;

private:
	CShape Shape;
	CShape* ShapeHead;
	CMath CM;
};



