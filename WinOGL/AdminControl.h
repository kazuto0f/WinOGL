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
	void CheckClosed();
	
	//交差判定
	bool CheckCrossShape();

private:
	double World_X;
	double World_Y;

	//選択される頂点
	CVertex* selectedV;

	//選択される面(shape)
	CShape* selectedS;

private:
	CShape Shape;
	CShape* ShapeHead;
	CMath CM;
	int Scount;
public:
	bool checkNaigai(CShape* TaisyouS, double x, double y);
	// 座標軸の描画
	void DrawAxis();
	//メニューやボタンの状態を管理する変数
	bool AxisFlag;
	bool EditFlag;
	CVertex* CheckSameVertex();
	// //面(shape)の選択の処理
	void CheckShapeSelect();
};



