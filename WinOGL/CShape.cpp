#include "pch.h"
#include "CShape.h"
#include<gl/GL.h>

CShape::CShape()
{
	Vertex_head = NULL;
	next_shape = NULL;
	front_shape = NULL;
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

//カウンタをセット
void CShape::SetCount(int t)
{
	count = t;
}

void CShape::SetClosed()
{
	if (!CheckCrossVertex(Vertex_head->GetNext()->GetX(), Vertex_head->GetNext()->GetY(), Start->GetX(), Start->GetY())) {
		SetStartPos();
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

void CShape::SetNotClosed()
{
	closed = false;
}

void CShape::SetShead(CVertex* v)
{
	Vertex_head = v;
}

//shapeの先頭を返却
CVertex* CShape::GetSHead()
{
	return Vertex_head;
}


//shapeの最初の点を返却
CVertex* CShape::GetStartVertex()
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
		Vertex_head->SetFront(New);
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

void CShape::SSetFront(CShape* front)
{
	//前の図形を示すポインタを書き込む
}

//前の図形のポインタを返却
CShape* CShape::SGetFront()
{
	return front_shape;
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
void CShape::DrawShape(float R, float G, float B)
{
	glColor3f(R,G,B);
	glPointSize(6);

	CVertex* nowV = Vertex_head;	//今の頂点
	CVertex* preV = Vertex_head;	//一つ前の頂点

	while (nowV != NULL) {			//今の頂点がNULLになるまで繰り返す
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



//面の描画
void CShape::DrawMen(float R, float G, float B)
{
	glColor3f(R, G, B);
	glPointSize(6);

	if (this->GetClosed()) {
		CVertex* froV = Vertex_head;	
		CVertex* midV = froV->GetNext();
		CVertex* endV = midV->GetNext();	

		/*double tx = Vertex_head->GetX();
		double ty = Vertex_head->GetY();
		for (CVertex* V = Vertex_head->GetNext(); V != NULL; V = V->GetNext()) {
			tx += V->GetX();
			ty += V->GetY();
		}
		tx = tx / this->GetCount();
		ty = ty / this->GetCount();

		while (nowV->GetNext() != NULL) {
			glBegin(GL_POLYGON);
			glVertex2f(nowV->GetX(),nowV->GetY());
			glVertex2f(nowV->GetNext()->GetX(), nowV->GetNext()->GetY());
			glVertex2f(tx, ty);
			glEnd();
			preV = nowV;
			nowV = nowV->GetNext();
		}*/

		glBegin(GL_POLYGON);
		while (endV != NULL) {
			glVertex2f(froV->GetX(), froV->GetY());
			glVertex2f(midV->GetX(), midV->GetY());
			glVertex2f(endV->GetX(), endV->GetY());
			froV = midV;
			midV = endV;
			endV = endV->GetNext();
		}
		glEnd();
	}
}

//点をスタート位置に設定
void CShape::SetStartPos()
{
	Vertex_head->SetXY(Start->GetX(), Start->GetY());
}

//点をスタート位置に設定
void CShape::SetStart(CVertex* v)
{
	Start = v;
}

//スタート位置を変更
void CShape::ChangeStart(CVertex* n)
{
	Start = n;
}


bool CShape::CheckCrossVertex(double sv_x, double sv_y, double ev_x, double ev_y)
{
	CVertex NewVE;
	CVertex NewVS;
	NewVS.SetXY(sv_x, sv_y);
	NewVE.SetXY(ev_x, ev_y);

	//ベクトルの登録
	CVect a;
	CVect a1;
	CVect a2;
	CVect b = CM.CalcVect(&NewVS, &NewVE);	//判定目的のベクトル
	CVect b1;
	CVect b2;

	if (count >= 2) {										//shapeの頂点数が2つ以上(nowVとpreVのため)
		CVertex* nowV = Vertex_head;
		CVertex* preV = nowV->GetNext();
		for (; preV != NULL; preV = preV->GetNext()) {		//preがNULLになるまで繰り返す
			if (CheckSameVertex(&NewVS, preV) ||			//同じ点か調べる（ベクトルの交差判定の際に同じ点からのベクトル同士の判定をなくすため）
				CheckSameVertex(&NewVS, nowV) ||
				CheckSameVertex(&NewVE, preV) ||
				CheckSameVertex(&NewVE, nowV)) 
			{
				nowV = nowV->GetNext();						//同じ点があるとき次のベクトルとの判定に進む
				continue;
			}

			//ベクトル設定
			a = CM.CalcVect(preV, nowV);
			a1 = CM.CalcVect(preV, &NewVS);
			a2 = CM.CalcVect(preV, &NewVE);
			b1 = CM.CalcVect(&NewVS, preV);
			b2 = CM.CalcVect(&NewVS, nowV);

			//外積計算
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

//同じ点かどうかを判別
bool CShape::CheckSameVertex(CVertex* v1, CVertex* v2)
{
	if (v1->GetX() == v2->GetX() && v1->GetY() == v2->GetY())
		return true;
	
	return false;
}

//同じ座標の頂点を返却
CVertex* CShape::GetSameVertex(double x, double y)
{	
	for (CVertex* now = GetSHead(); now != NULL; now = now->GetNext()) {
		if ((now->GetX() == x) && (now->GetY() == y)) {
			return now;
		}
	}
	return 0;
}

//SHAPE内の近い点の判別
CVertex* CShape::CheckSimilarVertex(double x, double y)
{
	for (CVertex* nowV = Vertex_head; nowV != NULL; nowV = nowV->GetNext()) {
		if (CM.euclid2p(nowV->GetX(), nowV->GetY(), x, y) < 0.04) {
			return nowV;
		}
	}
	return NULL;
}


// //内外判定
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





