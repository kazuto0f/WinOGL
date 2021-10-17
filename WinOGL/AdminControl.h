#pragma once
#include "CVertex.h"
#include "CShape.h"
#include "CMath.h"


class CAdminControl
{
public:
	CAdminControl();
	~CAdminControl();

	//OnDraw関数
	void OnDraw();

	//クリック座標の保存
	void OnClick(double,double,int,int);

	//Shapeの解放
	void FreeShape();

	//Vertexが初めの点と位置するか
	void CheckClosed(CShape* head);
	
	//交差判定
	bool CheckCrossShape(CShape* head);

private:
	double World_X;
	double World_Y;

private:
	CShape Shape;
	CShape* ShapeHead;
	CMath CM;
};



