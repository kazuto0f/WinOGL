#include "pch.h"
#include "AdminControl.h"
#include "CShape.h"
#include "CMath.h"
#include "CVect.h"
#include <math.h>
#include<gl/GL.h>


CAdminControl::CAdminControl()
{
	ShapeHead = NULL;
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
		nowS->DrawShape();
		preS = nowS;
		nowS = nowS->SGetNext();
	}
}

void CAdminControl::OnClick(double x,double y,int width,int height) 
{
	World_X = (x - 0.5) * 2;
	World_Y = (y - 0.5) * 2;

	if (width > height) {
		World_X = World_X * ((double)width / (double)height);
	}
	else {
		World_Y = World_Y * ((double)height / (double)width);
	}
	
	if (ShapeHead == NULL) {
		CShape* New = new CShape();
		New->SSetXY(World_X, World_Y);
		ShapeHead = New;
	}
	else
	{	
		CShape* nowS = ShapeHead;
		if (ShapeHead->GetClosed()) {
			CShape* New = new CShape();
			New->SSetXY(World_X, World_Y);
			New->SSetNext(ShapeHead);
			ShapeHead = New;
		}
		else 
		{
			/*if (!ShapeHead->CheckCrossVertex(World_X, World_Y)) {
				ShapeHead->SSetXY(World_X, World_Y);
				CheckClosed(ShapeHead);
			}*/
			if (!CheckCrossShape(ShapeHead)) {
				ShapeHead->SSetXY(World_X, World_Y);
				CheckClosed(ShapeHead);
			}
		}
	}
	
}

//}Œ`‚Ì‰ð•ú
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


//Vertex‚ª‰‚ß‚Ì“_‚Æˆê’v‚·‚é‚©
void CAdminControl::CheckClosed(CShape* head)
{
	if (head != NULL) {
		if (CM.euclid2p((head->GetStartVertex()->GetX()),(head->GetStartVertex()->GetY()), World_X, World_Y) < 0.06 ){
			if (head->GetCount() > 3) {
				head->SetClosed();
			}
		}
	}
}

//Œð·”»’è
bool CAdminControl::CheckCrossShape(CShape* head)
{
	CShape* now = head;
	
	for (; now != NULL; now = now->SGetNext()) {

		if (now->CheckCrossVertex(head->GetSHead()->GetX(), head->GetSHead()->GetY(), World_X, World_Y)) {
			return true;
		}
	}
	return false;
}



