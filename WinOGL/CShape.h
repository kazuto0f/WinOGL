#pragma once
#include "CVertex.h"
#include "CMath.h"
#include "CVect.h"
class CShape
{
public:
	CShape();
	~CShape();

private:
	//図形の始点
	CVertex* Start;
	//図形が閉じているか
	bool closed;
	//次の図形を示すポインタ
	CShape* next_shape;
	//頂点の先頭
	CVertex* Vertex_head;
	//カウンタ
	int count;

	CMath CM;


public:
	//カウンタを返却
	int GetCount();
	//closedにする
	void SetClosed();
	//shapeの先頭を返却
	CVertex* GetSHead();
	//shapeの最初の点を返却
	CVertex* GetStartVertex();
	//頂点のXY座標を書き込む
	void SSetXY(double,double);
	//次の頂点リストを指すポインタを書き込む
	void SSetNext(CShape*);
	//次の頂点リストを指すポインタを返却
	CShape* SGetNext();
	//閉じているかを返却
	bool GetClosed();
	//図形の解放
	void ShapeFree();
	//図形の描画
	void DrawShape();
	//点をスタート位置に設定
	void SetStart();
	//交差判定
	bool CheckCrossVertex(double, double);
	bool CheckCrossVertex(double, double, double, double);

	//同じ点かどうかを判別
	bool CheckSameVertex(CVertex*,CVertex*);
};

