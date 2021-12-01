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
	selectedL = NULL;
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

	if (ShapeHead != NULL && ShapeHead->GetClosed() != true) {

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
		if (selectedL != NULL) {
			glBegin(GL_LINES);
			glColor3f(1.0, 1.0, 0.0);
			glVertex2f(selectedL->GetX(), selectedL->GetY());
			glVertex2f(selectedL->GetNext()->GetX(), selectedL->GetNext()->GetY());
			glEnd();
		}
	}
	else {	//editFlag���I�t�̎��I��������
		selectedV = NULL;
		selectedS = NULL;
		selectedL = NULL;
	}
}

void CAdminControl::OnUp(double x, double y, int width, int height)
{
	if (selectedV != NULL) {	//selectedV��NULL�̎��͎��s���Ȃ�
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

		//�I�����ꂽ���_�̒l��ۑ����Ă���(����Ɉ������������Ƃ��ɖ߂�����)
		double temp_x = selectedV->GetX();
		double temp_y = selectedV->GetY();

		CShape* taisyouS = NULL;
		CVertex* selectedHead = NULL;

		//�I�����ꂽ���_�̈�O�̒��_
		CVertex* preSV = NULL;

		//EditFlag��true�̎��I������
		if (EditFlag) {
			if (ShapeHead == NULL) {
				//shape�������Ȃ��Ƃ�
				EditFlag = false;
			}
			else {
				if (CM.euclid2p(temp_x, temp_y, World_X, World_Y) > 0.03) {
					//�I�𒸓_���h���b�v�n�_�̍��W�ɐݒ�
					selectedV->SetXY(World_X, World_Y);


					//taisyouS�ɕύX���悤�Ƃ��Ă��钸�_���܂�shape������
					for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {

						//��ԍŏ��ɒǉ������_��I�����Ă���ꍇ�A�w�b�h��������,preSV�ɍŌ�̓_�̈�O��ǉ�����
						if ((temp_x == nowS->GetSHead()->GetX() && temp_y == nowS->GetSHead()->GetY())) {
							selectedHead = nowS->GetSHead();
							selectedHead->SetXY(World_X, World_Y);
							for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext()->GetNext() != NULL; nowV = nowV->GetNext()) {
								preSV = nowV;
							}
						}

						preSV = nowS->GetSHead();
						for (CVertex* nowV = nowS->GetSHead()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
							if ((nowV->GetX() == selectedV->GetX()) && (nowV->GetY() == selectedV->GetY())) {
								taisyouS = nowS;
								break;
							}
							preSV = nowV;
						}
						if (taisyouS != NULL)
							break;
					}

					//���O���肪false�̎�
					if (checkNaigai(taisyouS, World_X, World_Y)) {
						//�I�����ꂽ���_�̍��W�����Ƃɖ߂�
						selectedV->SetXY(temp_x, temp_y);
						if (selectedHead != NULL)
							selectedHead->SetXY(temp_x, temp_y);
					}
					else
					{
						//���ׂĂ̒��_�œ��O����
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
									//���O����Ɉ������������璸�_�����ɖ߂�
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									//�I�𒸓_�̏�����
									selectedV == NULL;
									break;
								}
							}
						}

						//��������
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {

							//�I�����ꂽ���_�̎��̒��_��NULL�o�Ȃ��Ƃ��̌�������
							if (selectedV->GetNext() != NULL) {
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), selectedV->GetNext()->GetX(), selectedV->GetNext()->GetY())) {
									//��������Ɉ������������璸�_�����ɖ߂�
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									//�I�𒸓_�̏�����
									selectedV == NULL;
									break;
								}
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//��������Ɉ������������璸�_�����ɖ߂�
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									//�I�𒸓_�̏�����
									selectedV == NULL;
									break;
								}
							}
							//�I�����ꂽ���_�̎��̒��_��NULL�̂Ƃ��̌�������
							else {
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//��������Ɉ������������璸�_�����ɖ߂�
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									//�I�𒸓_�̏�����
									selectedV == NULL;
									break;
								}
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), taisyouS->GetSHead()->GetNext()->GetX(), taisyouS->GetSHead()->GetNext()->GetY())) {
									//��������Ɉ������������璸�_�����ɖ߂�
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									
									//�I�𒸓_�̏�����
									selectedV == NULL;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

//���_�̑}��,�폜
void CAdminControl::insdelV(double x, double y, int width, int height)
{
	if (EditFlag) {

		//�f�o�C�X�̍��W����world���W�n�ɕϊ�
		World_X = (x - 0.5) * 2;
		World_Y = (y - 0.5) * 2;

		CShape* taisyouS = NULL;

		//��ʂ̔䂩��world���W���C��
		if (width > height) {
			World_X = World_X * ((double)width / (double)height);
		}
		else {
			World_Y = World_Y * ((double)height / (double)width);
		}

		if (selectedL != NULL) {
			//�I�����ꂽ�ӂƃN���b�N�����_�̃x�N�g�����v�Z
			CVect hen = CM.CalcVect(selectedL, selectedL->GetNext());
			CVect ten = CM.CalcVect(selectedL->GetX(), selectedL->GetY(), World_X, World_Y);

			//�N���b�N���W�ƑI�����ꂽ�����̃��[�N���b�h������0.03��菬�����Ƃ�
			if (CM.calcDistancePL(hen, ten) < 0.03) {

				//�ǉ����钸�_�𐶐�
				CVertex* New = new CVertex();

				CVect va = CM.CalcVect(selectedL,selectedL->GetNext());
				CVect vb = CM.CalcVect(selectedL->GetX(),selectedL->GetY(),World_X,World_Y);

				double s = (CM.calcNaiseki(vb, va) / CM.calcNaiseki(va, va));
				double t = 1 - s;

				//
				CVect a;
				a.setX(selectedL->GetNext()->GetX());
				a.setY(selectedL->GetNext()->GetY());

				CVect b;
				b.setX(selectedL->GetX());
				b.setY(selectedL->GetY());

				a.vectTimes(s);
				b.vectTimes(t);

				CVect q;
				q.setX(a.GetXVec() + b.GetXVec());
				q.setY(a.GetYVec() + b.GetYVec());

				New->SetXY(q.GetXVec(), q.GetYVec());
			
				New->SetNext(selectedL->GetNext());
				selectedL->SetNext(New);

				selectedL = NULL;
			}
		}
		else if(selectedV != NULL){
			//�N���b�N���W�ƍł��߂����_��ԋp
			CVertex* taisyouV =  CheckSameVertex();

			//�������W���`�F�b�N
			if (CM.euclid2p(taisyouV->GetX(),taisyouV->GetY(),World_X,World_Y) < 0.03) {
				
				CVertex* preV = NULL;
				//taisyouS�ɕύX���悤�Ƃ��Ă��钸�_���܂�shape������
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (nowV->GetX() == taisyouV->GetX(), nowV->GetY() == taisyouV->GetY()) {
							taisyouS = nowS;
							break;
						}
						preV = nowV;
					}
				}

				if (taisyouS->GetCount() > 2) {

					//�������Ƃ��Ă�����W���w�b�h�̎�
					if (taisyouS->GetSHead()->GetX() == taisyouV->GetX() && taisyouS->GetSHead()->GetY() == taisyouV->GetY()) {

					}
					else {
						preV->SetNext(taisyouV->GetNext());

						delete taisyouV;
						
						////���ׂĂ̒��_�œ��O����
						//for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
						//	for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						//		if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
						//			//���O����Ɉ������������璸�_�����ɖ߂�
						//			
						//			//�I�𒸓_�̏�����
						//			selectedV == NULL;
						//			break;
						//		}
						//	}
						//}
					}
				}

				
			}
			else {
				taisyouV = NULL;
				tais
			}
		}
	}
}

//�}�E�X�N���b�N�֐�
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
		selectedV = NULL;
		selectedL = NULL;
		selectedS = NULL;
		//shape�������Ȃ��Ƃ�
		if (ShapeHead == NULL) {
			EditFlag = false;
		}
		else {
		//���ׂĂ̐}�`�̒��_�̒��ł���臒l�ȉ��̈�ԋ߂��_��ԋp
			selectedV = CheckSameVertex();						//���_�A�Ő��A�`��̏��ɗD��x
			if (selectedV == NULL) {	//���_���I������Ă��Ȃ��Ƃ��ɗŐ��̑I�����ł���
				LineSelect();
			}
			if (selectedL == NULL && selectedV == NULL)	{	//���_�A�Ő����I������Ă��Ȃ��Ƃ��Ɍ`��̑I�����ł���
				CheckShapeSelect();
			}
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
				if (CM.euclid2p(ShapeHead->GetStartVertex()->GetX(), ShapeHead->GetStartVertex()->GetY(), World_X, World_Y) >= 0.03) {	//�ł����_���ŏ��̒��_�Ƌ߂��Ƃ��͌�����������Ȃ�
					if (!CheckCrossShape(ShapeHead, World_X,World_Y)) {			//���Ă��Ȃ��Ƃ��@��������
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
		if (CM.euclid2p((ShapeHead->GetStartVertex()->GetX()), (ShapeHead->GetStartVertex()->GetY()), World_X, World_Y) < 0.03) {	//�w�b�h��shape�̎n�_�ƃN���b�N���W�����[�N���b�h�����Ŕ���
			if (!CheckCrossShape(ShapeHead, World_X,World_Y)) {					//�X�^�[�g�n�_��u�����Ƃ��Ɍ�������
				if (ShapeHead->GetCount() > 3) {		//shape�ɒu���ꂽ���_�̐����R�ȏ�̎�
					if (Scount > 1) {					//shape�̃w�b�h���_�̎���NULL�̎�������
						ShapeHead->SetStart();			//�ꎞ�I��vertex_head���X�^�[�g�n�_�ɂ���
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
bool CAdminControl::CheckCrossShape(CShape* head, double x, double y)
{
	CShape* now = head;	//������shape
	
	for (; now != NULL; now = now->SGetNext()) {	//shape��NULL�ɂȂ�܂ŌJ��Ԃ�

		//���u�����Ƃ��Ă��钸�_�ƈ�O�ɂ��������_�Ԃ̃x�N�g����now��shape�Ɋ܂܂��x�N�g������������
		if (now->CheckCrossVertex(head->GetSHead()->GetX(), head->GetSHead()->GetY(), x, y)) {	
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
		for (CVertex* nowV = now->GetSHead()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
			if (CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X,World_Y) < 0.03) {
				if (minEuclid == NULL) {														//minEuclid�ɉ��������Ă��Ȃ��ꍇ�͂��������
					minEuclid = CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y);
					if (nowV == now->GetSHead() && now->GetClosed()) {
						while (nowV->GetNext() == NULL) {
							nowV = nowV->GetNext();
						}
					}
					MostCloseV = nowV;
				}
				else {
					if (minEuclid > CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y)) {
						minEuclid = CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y);
						if (nowV == now->GetSHead() && now->GetClosed()) {
						while (nowV->GetNext() == NULL) {
							nowV = nowV->GetNext();
						}
					}
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

//�Ő��̑I��
void CAdminControl::LineSelect()
{
	CShape* now = ShapeHead;
	CVertex* nowV = NULL;
	CVertex* nextV = NULL;
	CVertex* seL = NULL;
	double minD = NULL;
	//���ׂĂ̌`����J��Ԃ�
	for (; now != NULL; now = now->SGetNext()) {
		//�`��̒��̒��_���͂���(vhead)����J��Ԃ��B
		for (nowV = now->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
			nextV = nowV->GetNext();
			CVect va = CM.CalcVect(nowV, nextV);									//�Ő��x�N�g��
			CVect vb = CM.CalcVect(nowV->GetX(), nowV->GetY(), World_X, World_Y);	//�N���b�N�����_��nowV�̃x�N�g��
			double d = CM.calcDistancePL(va, vb);		//�_�ƒ����̋����̌v�Z
			if (d < 0.02) {
				//�_�ƒ����̋�����0.01�ȉ��̎�
				if (minD == NULL) {
					minD = d;
					CVect p = CM.CalcVect(nowV->GetX(), nowV->GetY(), World_X, World_Y);
					double s = CM.calcNaiseki(p, va) / CM.calcNaiseki(va, va);
					if (s >= 0 && s <= 1) {												//�����̒����͈̔͂̂ݔ��� |-|
						seL = nowV;
					}
				}
				else {
					if (minD > d) {
						minD = d;
						CVect p = CM.CalcVect(nowV->GetX(), nowV->GetY(), World_X, World_Y);
						double s = CM.calcNaiseki(p, va) / CM.calcNaiseki(va, va);
						if (s >= 0 && s <= 1) {												//�����̒����͈̔͂̂ݔ��� |-|
							seL = nowV;
						}
					}
				}
			}
		}
		selectedL = seL;		//�_�ƒ����̋�������ԋ߂�����selected�Ƃ��ēo�^
	}
}


//�}�E�X�𓮂����ƌĂ΂��
void CAdminControl::mouseMove(double x, double y, int width, int height)
{
	//�f�o�C�X�̍��W����world���W�n�ɕϊ�
	Cursor_Pos_X = (x - 0.5) * 2;
	Cursor_Pos_Y = (y - 0.5) * 2;

	//��ʂ̔䂩��world���W���C��
	if (width > height) {
		Cursor_Pos_X = Cursor_Pos_X * ((double)width / (double)height);
	}
	else {
		Cursor_Pos_Y = Cursor_Pos_Y * ((double)height / (double)width);
	}

	/*if (ShapeHead != NULL) {
		if(ShapeHead)
	}*/
}
