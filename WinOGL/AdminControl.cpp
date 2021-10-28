#include "pch.h"
#include "AdminControl.h"
#include "CShape.h"
#include "CMath.h"
#include "CVect.h"
#include "CVertex.h"
#include <math.h>
#include<gl/GL.h>


CAdminControl::CAdminControl()
{
	ShapeHead = NULL;
	Scount = 0;
	AxisFlag = false;
	EditFlag = false;
	selectedV = NULL;
	selectedS = NULL;
}

CAdminControl::~CAdminControl()
{
	FreeShape();
}



void CAdminControl::OnDraw() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT */);
	
	CShape* nowS = ShapeHead;
	CShape* preS = ShapeHead;

	while (nowS != NULL) {
		nowS->DrawShape(1.0,1.0,1.0);
		preS = nowS;
		nowS = nowS->SGetNext();
	}

	// AxisFlag��true�̂Ƃ����W����`�悷��
	if (AxisFlag) {
		DrawAxis();
	}

	//EditFlag��true�̎��I���������_�̕`�������
	if (EditFlag) {
		if (selectedV != NULL) {
			glColor3f(1.0, 0.0, 0.0);
			glPointSize(6);
			glBegin(GL_POINTS);
			glVertex2f(selectedV->GetX(), selectedV->GetY());
			glEnd();
		}
		if (selectedS != NULL) {
			selectedS->DrawShape(0.0, 1.0, 0.0);
		}
	}
	else {
		selectedV = NULL;
		selectedS = NULL;
	}
}

void CAdminControl::OnClick(double x,double y,int width,int height) 
{
	//�f�o�C�X�̍��W����world���W�n�ɕϊ�
	World_X = (x - 0.5) * 2;
	World_Y = (y - 0.5) * 2;

	//��ʂ̔䂩��world���W���C��
	if (width > height) {
		World_X = World_X * ((double)width / (double)height);
	}
	else {
		World_Y = World_Y * ((double)height / (double)width);
	}
	

	//EditFlag��true�̎��I������
	if (EditFlag) 
	{
		//shape�������Ȃ��Ƃ�
		if (ShapeHead == NULL) {
			EditFlag = false;
		}
		else {
		//���ׂĂ̐}�`�̒��_�̒��ł���臒l�ȉ��̈�ԋ߂��_��ԋp
			selectedV = CheckSameVertex();
			CheckShapeSelect();
		}
	}
	else{

		//Shape�̃w�b�h��NULL�̎���shape��V��������
		if (ShapeHead == NULL) {
			CShape* New = new CShape();
			Scount++;
			New->SSetXY(World_X, World_Y);
			//ShapeHead�̎����A�h���X��V������������shape�ɕύX
			ShapeHead = New;
		}
		//ShapeHead��NULL�łȂ��Ƃ�
		else
		{
			if (ShapeHead->GetClosed()) {					//ShapeHead�i�ŐV��Shape�����Ă��邩�𔻒�j				
				if (!checkNaigai(NULL, World_X, World_Y)) {
					CShape* New = new CShape();					//���Ă���Ƃ��ɐV����Shape�𐶐����N���b�N���W��o�^
					Scount++;
					New->SSetXY(World_X, World_Y);
					New->SSetNext(ShapeHead);
					ShapeHead = New;
				}
			}
			else
			{
				if (CM.euclid2p(ShapeHead->GetStartVertex()->GetX(), ShapeHead->GetStartVertex()->GetY(), World_X, World_Y) >= 0.05) {	//�ł����_���ŏ��̒��_�Ƌ߂��Ƃ��͌�����������Ȃ�
					if (!CheckCrossShape()) {			//���Ă��Ȃ��Ƃ��@��������
						if (!checkNaigai(ShapeHead, World_X, World_Y)) {
							ShapeHead->SSetXY(World_X, World_Y);	//�������肪false�̎��@�N���b�N���W��o�^
						}

					}
				}
				else {
					if (!checkNaigai(ShapeHead, World_X, World_Y)) {
						ShapeHead->SSetXY(World_X, World_Y);	//�������肪false�̎��@�N���b�N���W��o�^
						CheckClosed();
					}
				}
			}
		}
	}
}

//�}�`�̉��
void CAdminControl::FreeShape()
{
	CShape* temp = NULL;
	CShape* sp = ShapeHead;

	while (ShapeHead != NULL) {
		temp = ShapeHead->SGetNext();
		delete ShapeHead;
		ShapeHead = temp;
	}
}


//Vertex�����߂̓_�ƈ�v���邩
void CAdminControl::CheckClosed()
{
	if (ShapeHead != NULL) {
		if (CM.euclid2p((ShapeHead->GetStartVertex()->GetX()), (ShapeHead->GetStartVertex()->GetY()), World_X, World_Y) < 0.05) {	//�w�b�h��shape�̎n�_�ƃN���b�N���W�����[�N���b�h�����Ŕ���
			if (!CheckCrossShape()) {	//�X�^�[�g�n�_��u�����Ƃ��Ɍ�������
				if (ShapeHead->GetCount() > 3) { //shape�ɒu���ꂽ���_�̐����R�ȏ�̎�
					if (Scount > 1) {	//shape�̃w�b�h���_�̎���NULL�̎�������
						ShapeHead->SetStart();	//�ꎞ�I��vertex_head���X�^�[�g�n�_�ɂ���
						for (CShape* nowS = ShapeHead->SGetNext(); nowS != NULL; nowS = nowS->SGetNext()) {
							if (checkNaigai(nowS, nowS->GetSHead()->GetX(), nowS->GetSHead()->GetY())) {
								CVertex* temp = NULL;
								temp = ShapeHead->GetSHead()->GetNext();
								delete ShapeHead->GetSHead();
								ShapeHead->SetShead(temp);
								return;
							}
						}
					}
					ShapeHead->SetClosed();
				}
			}
		}
	}
}

//��������
bool CAdminControl::CheckCrossShape()
{
	CShape* now = ShapeHead;	//������shape
	
	for (; now != NULL; now = now->SGetNext()) {	//shape��NULL�ɂȂ�܂ŌJ��Ԃ�

		//���u�����Ƃ��Ă��钸�_�ƈ�O�ɂ��������_�Ԃ̃x�N�g����now��shape�Ɋ܂܂��x�N�g������������
		if (now->CheckCrossVertex(ShapeHead->GetSHead()->GetX(), ShapeHead->GetSHead()->GetY(), World_X, World_Y)) {	
			return true;
		}
	}
	return false;
}

//���O����
bool CAdminControl::checkNaigai(CShape* TaisyouS, double x, double y)
{
	CShape* now;	//������shape
	now = ShapeHead;
	for (; now != NULL; now = now->SGetNext()) {	//shape��NULL�ɂȂ�܂ŌJ��Ԃ�
		if (TaisyouS == now)						//�Ώۂ̒��_���܂܂��shape���Ȃ�
			continue;
		if (now->VCheckNaiGai(x, y)) {
			return true;
		}
	}

	return false;
}




// ���W���̕`��
void CAdminControl::DrawAxis()
{
	glBegin(GL_LINES);

	// x��
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	// y��
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);

	// z��
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}


//�N���b�N�����_�����_�ƈ�v���邩�`�F�b�N
CVertex* CAdminControl::CheckSameVertex()
{
	float minEuclid = NULL;
	CVertex* MostCloseV = NULL;
	CShape* now = ShapeHead;
	for (; now != NULL; now = now->SGetNext()) {
		for (CVertex* nowV = now->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
			if (CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X,World_Y) < 0.04) {
				if (minEuclid == NULL) {														//minEuclid�ɉ��������Ă��Ȃ��ꍇ�͂��������
					minEuclid = CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y);
					MostCloseV = nowV;
				}
				else {
					if (minEuclid > CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y)) {
						minEuclid = CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y);
						MostCloseV = nowV;
					}
				}
			}
		}
	}
	return MostCloseV;
}


// //��(shape)�̑I���̏���
void CAdminControl::CheckShapeSelect()
{
	CShape* now = ShapeHead;
	for (; now != NULL; now = now->SGetNext()) {	//shape��NULL�ɂȂ�܂ŌJ��Ԃ�
		if (now->GetClosed() == TRUE) {
			if (now->VCheckNaiGai(World_X, World_Y)) {
				selectedS = now;
				break;
			}
		}
	}
}
