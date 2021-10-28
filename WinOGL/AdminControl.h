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
	void CheckClosed();
	
	//��������
	bool CheckCrossShape();

private:
	double World_X;
	double World_Y;

	//�I������钸�_
	CVertex* selectedV;

	//�I��������(shape)
	CShape* selectedS;

private:
	CShape Shape;
	CShape* ShapeHead;
	CMath CM;
	int Scount;
public:
	bool checkNaigai(CShape* TaisyouS, double x, double y);
	// ���W���̕`��
	void DrawAxis();
	//���j���[��{�^���̏�Ԃ��Ǘ�����ϐ�
	bool AxisFlag;
	bool EditFlag;
	CVertex* CheckSameVertex();
	// //��(shape)�̑I���̏���
	void CheckShapeSelect();
};



