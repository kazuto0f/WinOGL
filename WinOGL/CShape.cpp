#include "pch.h"
#include "CShape.h"
#include<gl/GL.h>

CShape::CShape()
{
	Vertex_head = NULL;
	next_shape = NULL;
	Start = NULL;
	End = NULL;
	closed = false;
	count = 0;
	SSelected = false;
}


CShape::~CShape()
{
	ShapeFree();
}

int CShape::GetCount()
{
	return count;
}

//�J�E���^���Z�b�g
void CShape::SetCount(int t)
{
	count = t;
}

void CShape::SetClosed()
{
	if (!CheckCrossVertex(Vertex_head->GetNext()->GetX(), Vertex_head->GetNext()->GetY(), Start->GetX(), Start->GetY())) {
		SetStart();
		closed = true;
	}
	else {
		if (CheckCrossVertex(Vertex_head->GetNext()->GetX(), Vertex_head->GetNext()->GetY(), Vertex_head->GetX(), Vertex_head->GetY())) {
			CVertex* temp = NULL;
			temp = Vertex_head->GetNext();
			delete Vertex_head;
			Vertex_head = temp;
		}
	}
}

void CShape::SetShead(CVertex* v)
{
	Vertex_head = v;
}

//shape�̐擪��ԋp
CVertex* CShape::GetSHead()
{
	return Vertex_head;
}


//shape�̍ŏ��̓_��ԋp
CVertex* CShape::GetStartVertex()
{
	return Start;
}


//���_��XY���W����������
void CShape::SSetXY(double new_x, double new_y)
{
	CVertex* New = new CVertex();
	New->SetXY(new_x, new_y);
	count++;

	if (Vertex_head == NULL) {
		Vertex_head = New;
		Start = New;
	}
	else
	{
		New->SetNext(Vertex_head);
		Vertex_head = New;
	}
}


//���̒��_���X�g���w���|�C���^����������
void CShape::SSetNext(CShape* Shape_next)
{
	next_shape = Shape_next;
}

////���̒��_���X�g���w���|�C���^��ԋp
CShape* CShape::SGetNext()
{
	return next_shape;
}

//���Ă��邩��ԋp
bool CShape::GetClosed()
{
	return closed;
}

//�}�`�̉��
void CShape::ShapeFree()
{
	CVertex* temp = NULL;
	CVertex* vp = Vertex_head;

	while (Vertex_head != NULL)
	{
		temp = Vertex_head->GetNext();
		delete Vertex_head;
		Vertex_head = temp;
	}
}

//�}�`�̕`��
void CShape::DrawShape(float R, float G, float B)
{
	glColor3f(R,G,B);
	glPointSize(6);

	CVertex* nowV = Vertex_head;	//���̒��_
	CVertex* preV = Vertex_head;	//��O�̒��_

	while (nowV != NULL) {			//���̒��_��NULL�ɂȂ�܂ŌJ��Ԃ�
		glBegin(GL_POINTS);
		glVertex2f(nowV->GetX(), nowV->GetY());
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(nowV->GetX(), nowV->GetY());
		glVertex2f(preV->GetX(), preV->GetY());
		glEnd();
		preV = nowV;
		nowV = nowV->GetNext();
	}
}

//�_���X�^�[�g�ʒu�ɐݒ�
void CShape::SetStart()
{
	Vertex_head->SetXY(Start->GetX(), Start->GetY());
}

//�X�^�[�g�ʒu��ύX
void CShape::ChangeStart(CVertex* n)
{
	Start = n;
}

//��������
/*bool CShape::CheckCrossVertex(double click_x, double click_y)
{
	CVertex NewV;
	NewV.SetXY(click_x, click_y);
	CVect a;
	CVect a1;
	CVect a2;
	CVect b = CM.CalcVect(Vertex_head, &NewV);
	CVect b1;
	CVect b2;
	
	if (count > 2) {
		CVertex* nowV = Vertex_head->GetNext();
		CVertex* preV = nowV->GetNext();
		for (; preV != NULL; preV = preV->GetNext()) {
			if (NewV.GetX() == preV->GetX()&&NewV.GetY() == preV->GetY()) continue;
			a = CM.CalcVect(preV, nowV);
			a1 = CM.CalcVect(preV, Vertex_head);
			a2 = CM.CalcVect(preV, &NewV);
			b1 = CM.CalcVect(Vertex_head, preV);	
			b2 = CM.CalcVect(Vertex_head, nowV);

			double ca_1 = CM.calcGaiseki(a, a1);
			double ca_2 = CM.calcGaiseki(a, a2);
			double cb_1 = CM.calcGaiseki(b, b1);
			double cb_2 = CM.calcGaiseki(b, b2);

			if (ca_1 * ca_2 <= 0 && cb_1 * cb_2 <= 0) {
				return true;
			}
			nowV = nowV->GetNext();
		}
	}
	return false;
}*/

bool CShape::CheckCrossVertex(double sv_x, double sv_y, double ev_x, double ev_y)
{
	CVertex NewVE;
	CVertex NewVS;
	NewVS.SetXY(sv_x, sv_y);
	NewVE.SetXY(ev_x, ev_y);

	//�x�N�g���̓o�^
	CVect a;
	CVect a1;
	CVect a2;
	CVect b = CM.CalcVect(&NewVS, &NewVE);	//����ړI�̃x�N�g��
	CVect b1;
	CVect b2;

	if (count > 2) {										//shape�̒��_����2�ȏ�(nowV��preV�̂���)
		CVertex* nowV = Vertex_head;
		CVertex* preV = nowV->GetNext();
		for (; preV != NULL; preV = preV->GetNext()) {		//pre��NULL�ɂȂ�܂ŌJ��Ԃ�
			if (CheckSameVertex(&NewVS, preV) ||			//�����_�����ׂ�i�x�N�g���̌�������̍ۂɓ����_����̃x�N�g�����m�̔�����Ȃ������߁j
				CheckSameVertex(&NewVS, nowV) ||
				CheckSameVertex(&NewVE, preV) ||
				CheckSameVertex(&NewVE, nowV)) 
			{
				nowV = nowV->GetNext();						//�����_������Ƃ����̃x�N�g���Ƃ̔���ɐi��
				continue;
			}

			//�x�N�g���ݒ�
			a = CM.CalcVect(preV, nowV);
			a1 = CM.CalcVect(preV, &NewVS);
			a2 = CM.CalcVect(preV, &NewVE);
			b1 = CM.CalcVect(&NewVS, preV);
			b2 = CM.CalcVect(&NewVS, nowV);

			//�O�όv�Z
			double ca_1 = CM.calcGaiseki(a, a1);
			double ca_2 = CM.calcGaiseki(a, a2);
			double cb_1 = CM.calcGaiseki(b, b1);
			double cb_2 = CM.calcGaiseki(b, b2);

			if (ca_1 * ca_2 <= 0 && cb_1 * cb_2 <= 0) {
				return true;
			}
			nowV = nowV->GetNext();
		}
	}
	return false;
}

//�����_���ǂ����𔻕�
bool CShape::CheckSameVertex(CVertex* v1, CVertex* v2)
{
	if (v1->GetX() == v2->GetX() && v1->GetY() == v2->GetY())
		return true;
	
	return false;
}

//�������W�̒��_��ԋp
CVertex* CShape::GetSameVertex(double x, double y)
{	
	for (CVertex* now = GetSHead(); now != NULL; now = now->GetNext()) {
		if ((now->GetX() == x) && (now->GetY() == y)) {
			return now;
		}
	}
	return 0;
}

//SHAPE���̋߂��_�̔���
CVertex* CShape::CheckSimilarVertex(double x, double y)
{
	for (CVertex* nowV = Vertex_head; nowV != NULL; nowV = nowV->GetNext()) {
		if (CM.euclid2p(nowV->GetX(), nowV->GetY(), x, y) < 0.04) {
			return nowV;
		}
	}
	return NULL;
}


// //���O����
bool CShape::VCheckNaiGai(double x, double y)
{
	double sumAngle = 0;
	CVertex CheckV;
	CheckV.SetXY(x, y);

	CVertex* nowV = Vertex_head;
	CVertex* preV = Vertex_head->GetNext();
	for (; preV != NULL; preV = preV->GetNext()) 
	{
		sumAngle += CM.calcAngle(&CheckV, nowV, &CheckV, preV);

		nowV = nowV->GetNext();
	}
	if (abs(sumAngle) < 0.01)
		return false;
	return true;
}





