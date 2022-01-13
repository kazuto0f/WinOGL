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
	//図形の終点
	CVertex* End;
	//図形が閉じているか
	bool closed;
	//次の図形を示すポインタ
	CShape* next_shape;
	//前の図形を示すポインタ
	CShape* front_shape;
	//頂点の先頭
	CVertex* Vertex_head;
	//頂点の数
	int count;
	//形状が選択されているのか
	bool SSelected;

	CMath CM;


public:
	//カウンタを返却
	int GetCount();

	//カウンタをセット
	void SetCount(int t);

	//closedにする
	void SetClosed();
	//notclosedにする
	void SetNotClosed();

	//頂点ヘッドを設定
	void SetShead(CVertex* v);

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

	//前の図形を示すポインタを書き込む
	void SSetFront(CShape* front);

	//前の図形のポインタを返却
	CShape* SGetFront();

	//閉じているかを返却
	bool GetClosed();

	//図形の解放
	void ShapeFree();

	//図形の描画
	void DrawShape(float R,float G,float B);

	//面の描画
	void DrawMen(float R, float G, float B);

	//点をスタート位置に設定
	void SetStartPos();
	void SetStart(CVertex* v);

	//スタート位置を変更
	void ChangeStart(CVertex* n);

	//交差判定
	bool CheckCrossVertex(double, double);
	bool CheckCrossVertex(double, double, double, double);

	//同じ点かどうかを判別
	bool CheckSameVertex(CVertex*,CVertex*);

	//同じ点のアドレスを返す
	CVertex* GetSameVertex(double x, double y);

	//近い点かどうか判別
	CVertex* CheckSimilarVertex(double x, double y);

	//内外判定
	bool VCheckNaiGai(double x, double y);
	
};

