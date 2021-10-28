#include "pch.h"
#include "CVertex.h"

CVertex::CVertex()
{
	x = 0;
	y = 0;
	next_vertex = NULL;
}

CVertex::CVertex(double new_x,double new_y, CVertex* new_next)
{
	SetXY(new_x, new_y);
	SetNext(new_next);
}

CVertex::~CVertex()
{

}

//�@���_��X���W����������
void CVertex::SetX(double new_x)
{
	x = new_x;
}

//�@���_��Y���W����������
void CVertex::SetY(double new_y)
{
	y = new_y;
}

//�@���_��X�EY���W�̗�������������
void CVertex::SetXY(double new_x, double new_y)
{
	x = new_x;
	y = new_y;
}

//�@���_��X���W��ǂݍ��ލ���
double CVertex::GetX()
{
	return x;
}

//�@���_��Y���W��ǂݍ���
double CVertex::GetY()
{
	return y;
}

//�@���_��X�EY���W�̗�����ǂݍ���
void CVertex::GetXY(double* get_x, double* get_y)
{
	get_x = &x;
	get_y = &y;
}

//�@���̒��_���X�g���w���|�C���^����������
void CVertex::SetNext(CVertex* new_next)
{
	next_vertex = new_next;
}

//�@���̒��_���X�g���w���|�C���^��ǂݍ���
CVertex* CVertex::GetNext()
{
	return next_vertex;
}