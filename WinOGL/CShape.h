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
	//�}�`�̏I�_
	CVertex* End;
	//�}�`�����Ă��邩
	bool closed;
	//���̐}�`�������|�C���^
	CShape* next_shape;
	//�O�̐}�`�������|�C���^
	CShape* front_shape;
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

	//�J�E���^���Z�b�g
	void SetCount(int t);

	//closed�ɂ���
	void SetClosed();
	//notclosed�ɂ���
	void SetNotClosed();

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

	//�O�̐}�`�������|�C���^����������
	void SSetFront(CShape* front);

	//�O�̐}�`�̃|�C���^��ԋp
	CShape* SGetFront();

	//���Ă��邩��ԋp
	bool GetClosed();

	//�}�`�̉��
	void ShapeFree();

	//�}�`�̕`��
	void DrawShape(float R,float G,float B);

	//�ʂ̕`��
	void DrawMen(float R, float G, float B);

	//�_���X�^�[�g�ʒu�ɐݒ�
	void SetStartPos();
	void SetStart(CVertex* v);

	//�X�^�[�g�ʒu��ύX
	void ChangeStart(CVertex* n);

	//��������
	bool CheckCrossVertex(double, double);
	bool CheckCrossVertex(double, double, double, double);

	//�����_���ǂ����𔻕�
	bool CheckSameVertex(CVertex*,CVertex*);

	//�����_�̃A�h���X��Ԃ�
	CVertex* GetSameVertex(double x, double y);

	//�߂��_���ǂ�������
	CVertex* CheckSimilarVertex(double x, double y);

	//���O����
	bool VCheckNaiGai(double x, double y);
	
};

