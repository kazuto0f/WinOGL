#include "pch.h"
#include "AdminControl.h"
#include "CShape.h"
#include "CMath.h"
#include "CVect.h"
#include "CVertex.h"
#include <math.h>
#include<gl/GL.h>


CAdminControl::CAdminControl()
{
	ShapeHead = NULL;
	Scount = 0;
	AxisFlag = false;
	EditFlag = false;
	selectedV = NULL;
	selectedS = NULL;
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
		nowS->DrawShape(1.0,1.0,1.0);
		preS = nowS;
		nowS = nowS->SGetNext();
	}

	// AxisFlagがtrueのとき座標軸を描画する
	if (AxisFlag) {
		DrawAxis();
	}

	//EditFlagがtrueの時選択した頂点の描画をする
	if (EditFlag) {
		if (selectedV != NULL) {
			glColor3f(1.0, 0.0, 0.0);
			glPointSize(6);
			glBegin(GL_POINTS);
			glVertex2f(selectedV->GetX(), selectedV->GetY());
			glEnd();
		}
		if (selectedS != NULL) {
			selectedS->DrawShape(0.0, 1.0, 0.0);
		}
	}
	else {
		selectedV = NULL;
		selectedS = NULL;
	}
}

void CAdminControl::OnClick(double x,double y,int width,int height) 
{
	//デバイスの座標からworld座標系に変換
	World_X = (x - 0.5) * 2;
	World_Y = (y - 0.5) * 2;

	//画面の比からworld座標を修正
	if (width > height) {
		World_X = World_X * ((double)width / (double)height);
	}
	else {
		World_Y = World_Y * ((double)height / (double)width);
	}
	

	//EditFlagがtrueの時選択処理
	if (EditFlag) 
	{
		//shapeが何もないとき
		if (ShapeHead == NULL) {
			EditFlag = false;
		}
		else {
		//すべての図形の頂点の中である閾値以下の一番近い点を返却
			selectedV = CheckSameVertex();
			CheckShapeSelect();
		}
	}
	else{

		//ShapeのヘッドがNULLの時はshapeを新しく生成
		if (ShapeHead == NULL) {
			CShape* New = new CShape();
			Scount++;
			New->SSetXY(World_X, World_Y);
			//ShapeHeadの示すアドレスを新しく生成したshapeに変更
			ShapeHead = New;
		}
		//ShapeHeadがNULLでないとき
		else
		{
			if (ShapeHead->GetClosed()) {					//ShapeHead（最新のShapeが閉じているかを判定）				
				if (!checkNaigai(NULL, World_X, World_Y)) {
					CShape* New = new CShape();					//閉じているときに新しいShapeを生成しクリック座標を登録
					Scount++;
					New->SSetXY(World_X, World_Y);
					New->SSetNext(ShapeHead);
					ShapeHead = New;
				}
			}
			else
			{
				if (CM.euclid2p(ShapeHead->GetStartVertex()->GetX(), ShapeHead->GetStartVertex()->GetY(), World_X, World_Y) >= 0.05) {	//打った点が最初の頂点と近いときは交差判定をしない
					if (!CheckCrossShape()) {			//閉じていないとき　交差判定
						if (!checkNaigai(ShapeHead, World_X, World_Y)) {
							ShapeHead->SSetXY(World_X, World_Y);	//交差判定がfalseの時　クリック座標を登録
						}

					}
				}
				else {
					if (!checkNaigai(ShapeHead, World_X, World_Y)) {
						ShapeHead->SSetXY(World_X, World_Y);	//交差判定がfalseの時　クリック座標を登録
						CheckClosed();
					}
				}
			}
		}
	}
}

//図形の解放
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


//Vertexが初めの点と一致するか
void CAdminControl::CheckClosed()
{
	if (ShapeHead != NULL) {
		if (CM.euclid2p((ShapeHead->GetStartVertex()->GetX()), (ShapeHead->GetStartVertex()->GetY()), World_X, World_Y) < 0.05) {	//ヘッドのshapeの始点とクリック座標をユークリッド距離で判別
			if (!CheckCrossShape()) {	//スタート地点を置いたときに交差判定
				if (ShapeHead->GetCount() > 3) { //shapeに置かれた頂点の数が３つ以上の時
					if (Scount > 1) {	//shapeのヘッド頂点の次がNULLの時を除く
						ShapeHead->SetStart();	//一時的にvertex_headをスタート地点にする
						for (CShape* nowS = ShapeHead->SGetNext(); nowS != NULL; nowS = nowS->SGetNext()) {
							if (checkNaigai(nowS, nowS->GetSHead()->GetX(), nowS->GetSHead()->GetY())) {
								CVertex* temp = NULL;
								temp = ShapeHead->GetSHead()->GetNext();
								delete ShapeHead->GetSHead();
								ShapeHead->SetShead(temp);
								return;
							}
						}
					}
					ShapeHead->SetClosed();
				}
			}
		}
	}
}

//交差判定
bool CAdminControl::CheckCrossShape()
{
	CShape* now = ShapeHead;	//今居るshape
	
	for (; now != NULL; now = now->SGetNext()) {	//shapeがNULLになるまで繰り返す

		//今置こうとしている頂点と一つ前においた頂点間のベクトルとnowのshapeに含まれるベクトルを交差判定
		if (now->CheckCrossVertex(ShapeHead->GetSHead()->GetX(), ShapeHead->GetSHead()->GetY(), World_X, World_Y)) {	
			return true;
		}
	}
	return false;
}

//内外判定
bool CAdminControl::checkNaigai(CShape* TaisyouS, double x, double y)
{
	CShape* now;	//今居るshape
	now = ShapeHead;
	for (; now != NULL; now = now->SGetNext()) {	//shapeがNULLになるまで繰り返す
		if (TaisyouS == now)						//対象の頂点が含まれるshapeを省く
			continue;
		if (now->VCheckNaiGai(x, y)) {
			return true;
		}
	}

	return false;
}




// 座標軸の描画
void CAdminControl::DrawAxis()
{
	glBegin(GL_LINES);

	// x軸
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	// y軸
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);

	// z軸
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}


//クリックした点が頂点と一致するかチェック
CVertex* CAdminControl::CheckSameVertex()
{
	float minEuclid = NULL;
	CVertex* MostCloseV = NULL;
	CShape* now = ShapeHead;
	for (; now != NULL; now = now->SGetNext()) {
		for (CVertex* nowV = now->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
			if (CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X,World_Y) < 0.04) {
				if (minEuclid == NULL) {														//minEuclidに何も入っていない場合はそれを入れる
					minEuclid = CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y);
					MostCloseV = nowV;
				}
				else {
					if (minEuclid > CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y)) {
						minEuclid = CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y);
						MostCloseV = nowV;
					}
				}
			}
		}
	}
	return MostCloseV;
}


// //面(shape)の選択の処理
void CAdminControl::CheckShapeSelect()
{
	CShape* now = ShapeHead;
	for (; now != NULL; now = now->SGetNext()) {	//shapeがNULLになるまで繰り返す
		if (now->GetClosed() == TRUE) {
			if (now->VCheckNaiGai(World_X, World_Y)) {
				selectedS = now;
				break;
			}
		}
	}
}
