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

	//マウスを動かすと呼ばれる
	void mouseMove(double x, double y, int width, int height);

	//拡大関数
	void mouseWheel(short wheel);

	//回転関数
	void rotateShape(double x, double y);

	//マウスリリースの関数
	void OnUp(double x, double y, int width, int height);

	//頂点の挿入
	void insdelV(double x, double y, int width, int height);

	//Shapeの解放
	void FreeShape();

	void FreeShape(CShape);

	//Vertexが初めの点と位置するか
	void CheckClosed();
	
	//交差判定
	bool CheckCrossShape(CShape* head, double x, double y);


private:
	double World_X;
	double World_Y;

	double Cursor_Pos_X;
	double Cursor_Pos_Y;

	double Pre_Cursor_Pos_X;
	double Pre_Cursor_Pos_Y;


private:
	CShape Shape;
	CShape* ShapeHead;
	CShape CopyShape;	//形状のコピーを入れる
	CShape CopyShape1;	//面塗の際の3角形を入れる
	CMath CM;
	int Scount;

	//選択される頂点
	CVertex* selectedV;

	//選択される面(shape)
	CShape* selectedS;

	//選択される稜線(2つvertexの初めの方)
	CVertex* selectedL;

	//基点
	CVertex* basePoint;

public:
	bool checkNaigai(CShape* TaisyouS, double x, double y);

	bool checkNaigai2(CShape* TaisyouS, double x, double y);
	// 座標軸の描画
	void DrawAxis();
	//メニューやボタンの状態を管理する変数
	bool AxisFlag;
	bool EditFlag;
	bool Surface;
	//視点の変更を行う必要があるのかを判別するフラグ
	bool NeedPerspectiveChange;
	float Perspect_X;
	float Perspect_Y;
	CVertex* CheckSameVertex();
	// //面(shape)の選択の処理
	void CheckShapeSelect();
	//稜線の選択
	void LineSelect();
	//選択のリセット
	void resetSelect();
	//基点の設定
	void SetBasePoint(double x, double y, int width, int height);
	//基点の返却
	bool GetBasePoint();
	//形状が閉じているかを返す
	bool GetHeadShapeClosed();
	//全削除（リセット）
	void Reset();
	//形状のコピー
	void ShapeCopy(CShape* taisyouS);
	//面の描画
	void DrawSurface();
	void DrawSurfacePre(CShape* taisoyuS,double r, double g, double b);
	//視点移動
	void SetPerspectivePos(double x, double y, int width, int height);
	void ResetPerspectivePos();
	void MovePerspective();
};



