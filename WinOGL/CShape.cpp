#include "pch.h"
#include "CShape.h"
#include<gl/GL.h>

CShape::CShape()
{
	Vertex_head = NULL;
	next_shape = NULL;
	Start = NULL;
	closed = false;
	count = 0;
}


CShape::~CShape()
{
	ShapeFree();
}

int CShape::GetCount()
{
	return count;
}

void CShape::SetClosed()
{
	if (!CheckCrossVertex(Start->GetX(),Start->GetY())) {
		SetStart();
		closed = true;
	}
	else {
		CVertex* temp = NULL;
		temp = Vertex_head->GetNext();
		delete Vertex_head;
		Vertex_head = temp;
	}
}

//shapeの先頭を返却
CVertex* CShape::GetSHead()
{
	return Start;
}



//頂点のXY座標を書き込む
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


//次の頂点リストを指すポインタを書き込む
void CShape::SSetNext(CShape* Shape_next)
{
	next_shape = Shape_next;
}

////次の頂点リストを指すポインタを返却
CShape* CShape::SGetNext()
{
	return next_shape;
}

//閉じているかを返却
bool CShape::GetClosed()
{
	return closed;
}

//図形の解放
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

//図形の描写
void CShape::DrawShape()
{
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(20);

	CVertex* nowV = Vertex_head;
	CVertex* preV = Vertex_head;

	while (nowV != NULL) {
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

//点をスタート位置に設定
void CShape::SetStart()
{
	Vertex_head->SetXY(Start->GetX(), Start->GetY());
}

//交差判定
bool CShape::CheckCrossVertex(double click_x, double click_y)
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
}

