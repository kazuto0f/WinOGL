#pragma once
class CVertex
{
public:
	CVertex();
	CVertex(double new_x, double new_y, CVertex* new_next, CVertex* new_front);
	~CVertex();
private:
	//�@���_��X���W
	double x;
	//�@���_��Y���W
	double y;
	//�@���̒��_���X�g���w���|�C���^
	CVertex* next_vertex;
	//��O�������|�C���^
	CVertex* front_vertex;
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
	//�O�̒��_�̃��X�g�������|�C���^����������
	void SetFront(CVertex* new_front);
	//�@���̒��_���X�g���w���|�C���^��ǂݍ���
	CVertex* GetNext();
};

