#pragma once
class CVertex
{
public:
	CVertex();
	CVertex(double new_x, double new_y, CVertex* new_next);
	~CVertex();
private:
	//�@���_��X���W
	double x;
	//�@���_��Y���W
	double y;
	//�@���̒��_���X�g���w���|�C���^
	CVertex* next_vertex;
public:
	//�@���_��X���W����������
	void SetX(double new_x);
	//�@���_��Y���W����������
	void SetY(double new_y);
	//�@���_��X�EY���W�̗�������������
	void SetXY(double new_x, double new_y);
	//�@���_��X���W��ǂݍ��ލ���
	double GetX();
	//�@���_��Y���W��ǂݍ���
	double GetY();
	//�@���_��X�EY���W�̗�����ǂݍ���
	void GetXY(double* get_x, double* get_y);
	//�@���̒��_���X�g���w���|�C���^����������
	void SetNext(CVertex* new_next);
	//�@���̒��_���X�g���w���|�C���^��ǂݍ���
	CVertex* GetNext();
};

