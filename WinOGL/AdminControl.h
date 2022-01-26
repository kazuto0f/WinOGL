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

	//�}�E�X�𓮂����ƌĂ΂��
	void mouseMove(double x, double y, int width, int height);

	//�g��֐�
	void mouseWheel(short wheel);

	//��]�֐�
	void rotateShape(double x, double y);

	//�}�E�X�����[�X�̊֐�
	void OnUp(double x, double y, int width, int height);

	//���_�̑}��
	void insdelV(double x, double y, int width, int height);

	//Shape�̉��
	void FreeShape();

	void FreeShape(CShape);

	//Vertex�����߂̓_�ƈʒu���邩
	void CheckClosed();
	
	//��������
	bool CheckCrossShape(CShape* head, double x, double y);


private:
	double World_X;
	double World_Y;

	double Cursor_Pos_X;
	double Cursor_Pos_Y;

	double Pre_Cursor_Pos_X;
	double Pre_Cursor_Pos_Y;


private:
	CShape Shape;
	CShape* ShapeHead;
	CShape CopyShape;	//�`��̃R�s�[������
	CShape CopyShape1;	//�ʓh�̍ۂ�3�p�`������
	CMath CM;
	int Scount;

	//�I������钸�_
	CVertex* selectedV;

	//�I��������(shape)
	CShape* selectedS;

	//�I�������Ő�(2��vertex�̏��߂̕�)
	CVertex* selectedL;

	//��_
	CVertex* basePoint;

public:
	bool checkNaigai(CShape* TaisyouS, double x, double y);

	bool checkNaigai2(CShape* TaisyouS, double x, double y);
	// ���W���̕`��
	void DrawAxis();
	//���j���[��{�^���̏�Ԃ��Ǘ�����ϐ�
	bool AxisFlag;
	bool EditFlag;
	bool Surface;
	//���_�̕ύX���s���K�v������̂��𔻕ʂ���t���O
	bool NeedPerspectiveChange;
	float Perspect_X;
	float Perspect_Y;
	CVertex* CheckSameVertex();
	// //��(shape)�̑I���̏���
	void CheckShapeSelect();
	//�Ő��̑I��
	void LineSelect();
	//�I���̃��Z�b�g
	void resetSelect();
	//��_�̐ݒ�
	void SetBasePoint(double x, double y, int width, int height);
	//��_�̕ԋp
	bool GetBasePoint();
	//�`�󂪕��Ă��邩��Ԃ�
	bool GetHeadShapeClosed();
	//�S�폜�i���Z�b�g�j
	void Reset();
	//�`��̃R�s�[
	void ShapeCopy(CShape* taisyouS);
	//�ʂ̕`��
	void DrawSurface();
	void DrawSurfacePre(CShape* taisoyuS,double r, double g, double b);
	//���_�ړ�
	void SetPerspectivePos(double x, double y, int width, int height);
	void ResetPerspectivePos();
	void MovePerspective();
};



