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

	//マウスリリースの関数
	void OnUp(double x, double y, int width, int height);

	//Shapeの解放
	void FreeShape();

	//Vertexが初めの点と位置するか
	void CheckClosed();
	
	//交差判定
	bool CheckCrossShape(CShape* head, double x, double y);


private:
	double World_X;
	double World_Y;

	double Cursor_Pos_X;
	double Cursor_Pos_Y;

	//選択される頂点
	CVertex* selectedV;

	//選択される面(shape)
	CShape* selectedS;

	//選択される稜線(2つvertexの初めの方)
	CVertex* selectedL;

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
	//稜線の選択
	void LineSelect();
	
	//マウスを動かすと呼ばれる
	void mouseMove(double x, double y, int width, int height);
};



