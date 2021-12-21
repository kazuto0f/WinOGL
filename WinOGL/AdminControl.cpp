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
	basePoint = NULL;
}

CAdminControl::~CAdminControl()
{
	FreeShape();
	if (basePoint != NULL) {
		delete basePoint;
	}
}



void CAdminControl::OnDraw() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT */);

	CShape* nowS = ShapeHead;
	CShape* preS = ShapeHead;

	while (nowS != NULL) {
		nowS->DrawShape(1.0, 1.0, 1.0);
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
		if (basePoint != NULL) {
			glBegin(GL_POINTS);
			glColor3f(1.0, 0.0, 1.0);
			glVertex2f(basePoint->GetX(), basePoint->GetY());
			glEnd();
		}
	}
	//else {	//editFlag���I�t�̎��I��������
	//	selectedV = NULL;
	//	selectedS = NULL;
	//	selectedL = NULL;
	//	/*if (ShapeHead != NULL || selectedS->GetBasePoint() != NULL) {
	//		delete selectedS->GetBasePoint();
	//	}*/
//}
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

		//���������
		bool ret = false;

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

					//���O����
					//shape�������Ă��Ȃ��Ƃ���shapehead�̎����画��
					if (!ShapeHead->GetClosed()) {
						//���ׂĂ̒��_�œ��O����
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
							}
							if (ret)break;
						}

						//��������
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {

							//�I�����ꂽ���_�̎��̒��_��NULL�łȂ��Ƃ��̌�������
							if (selectedV->GetNext() != NULL) {
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), selectedV->GetNext()->GetX(), selectedV->GetNext()->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
							}
							//�I�����ꂽ���_�̎��̒��_��NULL�̂Ƃ��̌�������
							else {
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), taisyouS->GetSHead()->GetNext()->GetX(), taisyouS->GetSHead()->GetNext()->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
							}
						}
					}
					//shape�������Ă���Ƃ�
					/*else if (checkNaigai(taisyouS, World_X, World_Y)) {
						ret = true;
					}*/
					else
					{
						//���ׂĂ̒��_�œ��O����
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
							}
							if (ret)break;
						}

						//��������
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {

							//�I�����ꂽ���_�̎��̒��_��NULL�o�Ȃ��Ƃ��̌�������
							if (selectedV->GetNext() != NULL) {
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), selectedV->GetNext()->GetX(), selectedV->GetNext()->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
							}
							//�I�����ꂽ���_�̎��̒��_��NULL�̂Ƃ��̌�������
							else {
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), taisyouS->GetSHead()->GetNext()->GetX(), taisyouS->GetSHead()->GetNext()->GetY())) {
									//��������Ɉ�������������ret��true
									ret = true;
									break;
								}
							}
						}
					}

					if (ret) {
						//�I�����ꂽ���_�̍��W�����Ƃɖ߂�
						selectedV->SetXY(temp_x, temp_y);
						if (selectedHead != NULL)
							selectedHead->SetXY(temp_x, temp_y);

						//�I�𒸓_�̏�����
						selectedV == NULL;
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
		//�߂����𔻕ʂ���t���O
		bool ret = false;

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
			//taisyouS�ɕύX���悤�Ƃ��Ă��钸�_���܂�shape������
			for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
				for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
					if (nowV->GetX() == selectedL->GetX(), nowV->GetY() == selectedL->GetY()) {
						taisyouS = nowS;
						break;
					}
				}
				if (taisyouS != NULL)	break;
			}

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

				taisyouS->SetCount(taisyouS->GetCount() + 1);

				selectedL = NULL;
			}
		}
		else if (selectedV != NULL) {
			CVertex* preV = NULL;
			CVertex* PreEnd = NULL;

			//�`�󂪕����Ă��Ȃ��Ƃ�
			if (!ShapeHead->GetClosed()) {
				//taisyouS�ɕύX���悤�Ƃ��Ă��钸�_���܂�shape������
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					preV = nowS->GetSHead();
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (nowV->GetX() == selectedV->GetX(), nowV->GetY() == selectedV->GetY()) {
							taisyouS = nowS;
							break;
						}
						preV = nowV;
					}
					if (taisyouS != NULL)	break;
				}

				//�`��̍ŏI�̈�O��PreEnd�ɓ����
				//if (taisyouS != ShapeHead) {
					for (CVertex* nowV = taisyouS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
						if (nowV->GetNext()->GetNext() == NULL) {
							PreEnd = nowV;
						}
					}
				//}
			}
			else {
				//taisyouS�ɕύX���悤�Ƃ��Ă��钸�_���܂�shape������
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					preV = nowS->GetSHead();
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (nowV->GetX() == selectedV->GetX(), nowV->GetY() == selectedV->GetY()) {
							taisyouS = nowS;
							break;
						}
						preV = nowV;
					}
					if (taisyouS != NULL)	break;
				}

				//�`��̍ŏI�̈�O��PreEnd�ɓ����
				for (CVertex* nowV = taisyouS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
					if (nowV->GetNext()->GetNext() == NULL) {
						PreEnd = nowV;
					}
				}
			}

			bool delhead = false;


			if (taisyouS->GetCount() > 4) {
				//�������Ƃ��Ă�����W���w�b�h�̎�
				if (taisyouS->GetSHead()->GetX() == selectedV->GetX() && taisyouS->GetSHead()->GetY() == selectedV->GetY()) {

					delhead = true;

					//���_�w�b�h,�I�_�̈ꎞ�ۑ�
					CVertex* tempHead = taisyouS->GetSHead();
					CVertex* tempEnd = PreEnd->GetNext();

					//�`��̃w�b�h������炷
					taisyouS->SetShead(taisyouS->GetSHead()->GetNext());

					//�i���_�}���j�V�����w�b�h�ɐV�����I�_�𐶐����Ȃ���
					CVertex* New = new CVertex();
					New->SetXY(taisyouS->GetSHead()->GetX(), taisyouS->GetSHead()->GetY());
					PreEnd->SetNext(New);

					//�`��̒��_���̍Đݒ�
					taisyouS->SetCount(taisyouS->GetCount() - 1);

					//if (!ShapeHead->GetClosed()) {
					//	//���O����
					//	for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
					//		for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
					//			//���O���肪true�̎�
					//			if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
					//				//���O����ɓ������t���O
					//				ret = true;
					//			}
					//		}
					//	}

					//	//���O���肪false�̂Ƃ����Ȍ�������
					//	if (!ret) {
					//		if (taisyouS->CheckCrossVertex(PreEnd->GetX(), PreEnd->GetY(), preV->GetX(), preV->GetY()) == true)
					//			ret = true;
					//	}

					//	//�폜�n�̏���
					//	if (ret == true) {
					//		//���_�̐���߂�
					//		taisyouS->SetCount(taisyouS->GetCount() + 1);
					//		//�I�_��߂�
					//		PreEnd->SetNext(tempEnd);
					//		//�w�b�h��߂�
					//		taisyouS->SetShead(tempHead);
					//		//�ǉ������_���폜
					//		delete New;
					//	}
					//	else {
					//		delete tempHead;
					//		delete tempEnd;
					//	}
					//}
					//else {

					//���O����
					if (!ShapeHead->GetClosed()) {
						for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								//���O���肪true�̎�
								if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
									//���O����ɓ������t���O
									ret = true;
								}
							}
						}
					}
					else {
						for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								//���O���肪true�̎�
								if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
									//���O����ɓ������t���O
									ret = true;
								}
							}
						}
					}


					//���Ȍ�������

					if (taisyouS->CheckCrossVertex(PreEnd->GetX(), PreEnd->GetY(), preV->GetX(), preV->GetY()) == true)
						ret = true;

					//��������
					for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
						for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
							for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
								if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
									ret = true;
									break;
								}
							}
							if (ret)	break;
						}
						if (ret)	break;
					}


					//�폜�n�̏���
					if (ret == true) {
						//���_�̐���߂�
						taisyouS->SetCount(taisyouS->GetCount() + 1);
						//�I�_��߂�
						PreEnd->SetNext(tempEnd);
						//�w�b�h��߂�
						taisyouS->SetShead(tempHead);
						//�ǉ������_���폜
						delete New;
					}
					else {
						delete tempHead;
						delete tempEnd;
					}
					//}
				}
				else {

					//���_�̑}��
					preV->SetNext(selectedV->GetNext());
					//���_���̍Đݒ�
					taisyouS->SetCount(taisyouS->GetCount() - 1);

					//���O����
					if (ShapeHead->GetClosed()) {
						for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
									//���O���肪true�̎�

									//���_�̐���߂�
									taisyouS->SetCount(taisyouS->GetCount() + 1);
									//�I���������_�����X�g�ɖ߂�
									preV->SetNext(selectedV);
									//���O����ɓ������t���O
									ret = true;
								}
							}
						}
					}
					else {
						for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
									//���O���肪true�̎�

									//���_�̐���߂�
									taisyouS->SetCount(taisyouS->GetCount() + 1);
									//�I���������_�����X�g�ɖ߂�
									preV->SetNext(selectedV);
									//���O����ɓ������t���O
									ret = true;
								}
							}
						}
					}

					//���Ȍ�������

					if (taisyouS->CheckCrossVertex(preV->GetX(), preV->GetY(), preV->GetNext()->GetX(), preV->GetNext()->GetY()) == true)
						ret = true;

					//��������
					for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
						for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
							for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
								if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
									ret = true;
									break;
								}
							}
							if (ret)	break;
						}
						if (ret)	break;
					}

					//�폜�n�̏���
					if (!ret) {
						delete selectedV;
					}
					else {
						preV->SetNext(selectedV);
						taisyouS->SetCount(taisyouS->GetCount() + 1);
					}
				}
			}
		}
		selectedV = NULL;
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
		/*if (basePoint != NULL) {
			delete basePoint;
			basePoint = NULL;
		}*/
		
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
			if (selectedL == NULL && selectedV == NULL) {	//���_�A�Ő����I������Ă��Ȃ��Ƃ��Ɍ`��̑I�����ł���
				CheckShapeSelect();
			}
		}
	}
	else{
		//��_�̃��Z�b�g
		delete basePoint;
		basePoint = NULL;

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
					ShapeHead->SSetFront(New);
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

// �}�E�X�𓮂����ƌĂ΂��(���N���b�N�������ꂽ���)
void CAdminControl::mouseMove(double x, double y, int width, int height)
{
	//���O�܂��͌������������
	bool ret = false;
	//�`�󂪑I������Ă��鎞
	if (selectedS != NULL) {

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

		//��_������ꍇ
		if (basePoint != NULL) {
			rotateShape(Cursor_Pos_X, Pre_Cursor_Pos_Y);
		}
		//��_���Ȃ��ꍇ
		else {
			//�}�`�̒��S���W���v�Z
			double tx = selectedS->GetSHead()->GetX();
			double ty = selectedS->GetSHead()->GetY();
			for (CVertex* nowV = selectedS->GetSHead()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
				tx += nowV->GetX();
				ty += nowV->GetY();
			}
			tx = tx / selectedS->GetCount();
			ty = ty / selectedS->GetCount();



			//���W�ɒ��S���W�ƃ}�E�X���W�̍��𑫂�
			for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
				nowV->SetX(nowV->GetX() + (Cursor_Pos_X - tx));
				nowV->SetY(nowV->GetY() + (Cursor_Pos_Y - ty));
			}

			if (!ShapeHead->GetClosed()) {
				//���O����
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
			}
			else {
				//���O����
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
			}

			//��������
			for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
				for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
					for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
						if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
				if (ret)	break;
			}

			if (ret) {
				for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
					nowV->SetX(nowV->GetX() - (Cursor_Pos_X - tx));
					nowV->SetY(nowV->GetY() - (Cursor_Pos_Y - ty));
				}
			}

		}
		Pre_Cursor_Pos_X = Cursor_Pos_X;
		Pre_Cursor_Pos_Y = Cursor_Pos_Y;
	}
}

//�g��֐�
void CAdminControl::mouseWheel(short wheel)
{
	bool ret = false;

	if (selectedS != NULL) {
		if (basePoint != NULL) {
			for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
				if (wheel > 0) {
					CVect V = CM.CalcVect(basePoint,nowV);
					nowV->SetX(1.03 * ((nowV->GetX()) - (basePoint->GetX())) + (basePoint->GetX()));
					nowV->SetY(1.03 * ((nowV->GetY()) - (basePoint->GetY())) + (basePoint->GetY()));
				}
				else {
					CVect V = CM.CalcVect(basePoint, nowV);
					nowV->SetX(0.97087379 * ((nowV->GetX()) - (basePoint->GetX())) + (basePoint->GetX()));
					nowV->SetY(0.97087379 * ((nowV->GetY()) - (basePoint->GetY())) + (basePoint->GetY()));
				}
			}

			if (!ShapeHead->GetClosed()) {
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
			}
			else {

				// ���O����
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
			}

			//��������
			for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
				for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
					for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
						if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
				if (ret)	break;
			}

			//ret �� true�̂Ƃ����ɖ߂�
			if (ret) {
				for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
					if (wheel > 0) {
						CVect V = CM.CalcVect(basePoint, nowV);
						nowV->SetX(0.97087379 * ((nowV->GetX()) - (basePoint->GetX())) + (basePoint->GetX()));
						nowV->SetY(0.97087379 * ((nowV->GetY()) - (basePoint->GetY())) + (basePoint->GetY()));
					}
					else {
						CVect V = CM.CalcVect(basePoint, nowV);
						nowV->SetX(1.03 * ((nowV->GetX()) - (basePoint->GetX())) + (basePoint->GetX()));
						nowV->SetY(1.03 * ((nowV->GetY()) - (basePoint->GetY())) + (basePoint->GetY()));
					}
				}
			}
		}
	}
}

//��]�֐�
void CAdminControl::rotateShape(double x, double y)
{
	bool ret = false;

	if (selectedS != NULL) {
		if (basePoint != NULL) {

			CVect bm = CM.CalcVect(basePoint->GetX(), basePoint->GetY(), x, y);
			CVect bv = CM.CalcVect(basePoint, selectedS->GetSHead());
			double rad = CM.calcAngle(bv, bm);

			for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {

				double x2 = ((nowV->GetX() - basePoint->GetX()) * cos(rad)) - ((nowV->GetY() - basePoint->GetY()) * sin(rad)) + basePoint->GetX();
				double y2 = ((nowV->GetX() - basePoint->GetX()) * sin(rad)) + ((nowV->GetY() - basePoint->GetY()) * cos(rad)) + basePoint->GetY();

				nowV->SetXY(x2, y2);
			}
			if (!ShapeHead->GetClosed()) {
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}

					if (ret)	break;
				}
			}
			else {

				// ���O����
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}

					if (ret)	break;
				}
			}

			//��������
			for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
				for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
					for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
						if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
				if (ret)	break;
			}

			//����Ɉ������������猳�ɖ߂�
			if (ret) {
				for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {

					double x2 = ((nowV->GetX() - basePoint->GetX()) * cos(-rad)) - ((nowV->GetY() - basePoint->GetY()) * sin(-rad)) + basePoint->GetX();
					double y2 = ((nowV->GetX() - basePoint->GetX()) * sin(-rad)) + ((nowV->GetY() - basePoint->GetY()) * cos(-rad)) + basePoint->GetY();

					nowV->SetXY(x2, y2);
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
						ShapeHead->SetStartPos();			//�ꎞ�I��vertex_head���X�^�[�g�n�_�ɂ���
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

//�`�󂪕����Ă��Ȃ��Ƃ��̓��O����
bool CAdminControl::checkNaigai2(CShape* TaisyouS, double x, double y)
{
	CShape* now;	//������shape
	now = ShapeHead;
	for (; now != NULL; now = now->SGetNext()) {	//shape��NULL�ɂȂ�܂ŌJ��Ԃ�
		if (TaisyouS == now)						//�Ώۂ̒��_���܂܂��shape���Ȃ�
			continue;
		if (ShapeHead == now)
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

//�I���̃��Z�b�g
void CAdminControl::resetSelect()
{	
	if (basePoint != NULL) {
		delete basePoint;
	}
	basePoint = NULL;
	selectedV = NULL;
	selectedL = NULL;
	selectedS = NULL;
}

//��_�̐ݒ�
void CAdminControl::SetBasePoint(double x, double y, int width, int height)
{
	//�f�o�C�X�̍��W����world���W�n�ɕϊ�
	x = (x - 0.5) * 2;
	y = (y - 0.5) * 2;

	//��ʂ̔䂩��world���W���C��
	if (width > height) {
		x = x * ((double)width / (double)height);
	}
	else {
		y = y * ((double)height / (double)width);
	}

	//basepoint�ɉ��������Ă���Ƃ�
	if (basePoint != NULL) {
		//�N���b�N���W��basepoint�Ƌ߂��Ƃ�
		if (CM.euclid2p(basePoint->GetX(), basePoint->GetY(), x, y) < 0.05) {
			delete basePoint;
			basePoint = NULL;
		}
		//�����Ƃ��͈ړ�������
		else {
			basePoint->SetXY(x, y);
		}
	}
	//NUL�̂Ƃ��͐V��������
	else {
		CVertex* New = new CVertex();
		New->SetXY(x, y);
		basePoint = New;
	}
}

//��_�̕ԋp
bool CAdminControl::GetBasePoint() {
	if (basePoint != NULL) {
		return true;
	}
	else {
		return false;
	}
}



