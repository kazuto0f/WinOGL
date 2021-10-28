#pragma once
#include "CVertex.h"
#include "CMath.h"
#include "CVect.h"
class CShape
{
public:
	CShape();
	~CShape();

private:
	//�}�`�̎n�_
	CVertex* Start;
	//�}�`�����Ă��邩
	bool closed;
	//���̐}�`�������|�C���^
	CShape* next_shape;
	//���_�̐擪
	CVertex* Vertex_head;
	//���_�̐�
	int count;
	//�`�󂪑I������Ă���̂�
	bool SSelected;

	CMath CM;


public:
	//�J�E���^��ԋp
	int GetCount();

	//closed�ɂ���
	void SetClosed();

	//���_�w�b�h��ݒ�
	void SetShead(CVertex* v);

	//shape�̐擪��ԋp
	CVertex* GetSHead();

	//shape�̍ŏ��̓_��ԋp
	CVertex* GetStartVertex();

	//���_��XY���W����������
	void SSetXY(double,double);

	//���̒��_���X�g���w���|�C���^����������
	void SSetNext(CShape*);

	//���̒��_���X�g���w���|�C���^��ԋp
	CShape* SGetNext();

	//���Ă��邩��ԋp
	bool GetClosed();

	//�}�`�̉��
	void ShapeFree();

	//�}�`�̕`��
	void DrawShape(float R,float G,float B);

	//�_���X�^�[�g�ʒu�ɐݒ�
	void SetStart();

	//��������
	bool CheckCrossVertex(double, double);
	bool CheckCrossVertex(double, double, double, double);

	//�����_���ǂ����𔻕�
	bool CheckSameVertex(CVertex*,CVertex*);

	//�߂��_���ǂ�������
	CVertex* CheckSimilarVertex(double x, double y);

	// //���O����
	bool VCheckNaiGai(double x, double y);
	
};

