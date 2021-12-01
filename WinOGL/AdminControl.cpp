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
	selectedL = NULL;
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

	if (ShapeHead != NULL && ShapeHead->GetClosed() != true) {

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
		if (selectedL != NULL) {
			glBegin(GL_LINES);
			glColor3f(1.0, 1.0, 0.0);
			glVertex2f(selectedL->GetX(), selectedL->GetY());
			glVertex2f(selectedL->GetNext()->GetX(), selectedL->GetNext()->GetY());
			glEnd();
		}
	}
	else {	//editFlagがオフの時選択を解除
		selectedV = NULL;
		selectedS = NULL;
		selectedL = NULL;
	}
}

void CAdminControl::OnUp(double x, double y, int width, int height)
{
	if (selectedV != NULL) {	//selectedVがNULLの時は実行しない
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

		//選択された頂点の値を保存しておく(判定に引っかかったときに戻すため)
		double temp_x = selectedV->GetX();
		double temp_y = selectedV->GetY();

		CShape* taisyouS = NULL;
		CVertex* selectedHead = NULL;

		//選択された頂点の一つ前の頂点
		CVertex* preSV = NULL;

		//EditFlagがtrueの時選択処理
		if (EditFlag) {
			if (ShapeHead == NULL) {
				//shapeが何もないとき
				EditFlag = false;
			}
			else {
				if (CM.euclid2p(temp_x, temp_y, World_X, World_Y) > 0.03) {
					//選択頂点をドロップ地点の座標に設定
					selectedV->SetXY(World_X, World_Y);


					//taisyouSに変更しようとしている頂点を含むshapeを入れる
					for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {

						//一番最初に追加した点を選択している場合、ヘッドも動かす,preSVに最後の点の一つ前を追加する
						if ((temp_x == nowS->GetSHead()->GetX() && temp_y == nowS->GetSHead()->GetY())) {
							selectedHead = nowS->GetSHead();
							selectedHead->SetXY(World_X, World_Y);
							for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext()->GetNext() != NULL; nowV = nowV->GetNext()) {
								preSV = nowV;
							}
						}

						preSV = nowS->GetSHead();
						for (CVertex* nowV = nowS->GetSHead()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
							if ((nowV->GetX() == selectedV->GetX()) && (nowV->GetY() == selectedV->GetY())) {
								taisyouS = nowS;
								break;
							}
							preSV = nowV;
						}
						if (taisyouS != NULL)
							break;
					}

					//内外判定がfalseの時
					if (checkNaigai(taisyouS, World_X, World_Y)) {
						//選択された頂点の座標をもとに戻す
						selectedV->SetXY(temp_x, temp_y);
						if (selectedHead != NULL)
							selectedHead->SetXY(temp_x, temp_y);
					}
					else
					{
						//すべての頂点で内外判定
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
									//内外判定に引っかかったら頂点を元に戻す
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									//選択頂点の初期化
									selectedV == NULL;
									break;
								}
							}
						}

						//交差判定
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {

							//選択された頂点の次の頂点がNULL出ないときの交差判定
							if (selectedV->GetNext() != NULL) {
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), selectedV->GetNext()->GetX(), selectedV->GetNext()->GetY())) {
									//交差判定に引っかかったら頂点を元に戻す
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									//選択頂点の初期化
									selectedV == NULL;
									break;
								}
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//交差判定に引っかかったら頂点を元に戻す
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									//選択頂点の初期化
									selectedV == NULL;
									break;
								}
							}
							//選択された頂点の次の頂点がNULLのときの交差判定
							else {
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//交差判定に引っかかったら頂点を元に戻す
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									//選択頂点の初期化
									selectedV == NULL;
									break;
								}
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), taisyouS->GetSHead()->GetNext()->GetX(), taisyouS->GetSHead()->GetNext()->GetY())) {
									//交差判定に引っかかったら頂点を元に戻す
									selectedV->SetXY(temp_x, temp_y);
									if (selectedHead != NULL)
										selectedHead->SetXY(temp_x, temp_y);
									
									//選択頂点の初期化
									selectedV == NULL;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

//頂点の挿入,削除
void CAdminControl::insdelV(double x, double y, int width, int height)
{
	if (EditFlag) {

		//デバイスの座標からworld座標系に変換
		World_X = (x - 0.5) * 2;
		World_Y = (y - 0.5) * 2;

		CShape* taisyouS = NULL;

		//画面の比からworld座標を修正
		if (width > height) {
			World_X = World_X * ((double)width / (double)height);
		}
		else {
			World_Y = World_Y * ((double)height / (double)width);
		}

		if (selectedL != NULL) {
			//選択された辺とクリックした点のベクトルを計算
			CVect hen = CM.CalcVect(selectedL, selectedL->GetNext());
			CVect ten = CM.CalcVect(selectedL->GetX(), selectedL->GetY(), World_X, World_Y);

			//クリック座標と選択された線分のユークリッド距離が0.03より小さいとき
			if (CM.calcDistancePL(hen, ten) < 0.03) {

				//追加する頂点を生成
				CVertex* New = new CVertex();

				CVect va = CM.CalcVect(selectedL,selectedL->GetNext());
				CVect vb = CM.CalcVect(selectedL->GetX(),selectedL->GetY(),World_X,World_Y);

				double s = (CM.calcNaiseki(vb, va) / CM.calcNaiseki(va, va));
				double t = 1 - s;

				//
				CVect a;
				a.setX(selectedL->GetNext()->GetX());
				a.setY(selectedL->GetNext()->GetY());

				CVect b;
				b.setX(selectedL->GetX());
				b.setY(selectedL->GetY());

				a.vectTimes(s);
				b.vectTimes(t);

				CVect q;
				q.setX(a.GetXVec() + b.GetXVec());
				q.setY(a.GetYVec() + b.GetYVec());

				New->SetXY(q.GetXVec(), q.GetYVec());
			
				New->SetNext(selectedL->GetNext());
				selectedL->SetNext(New);

				selectedL = NULL;
			}
		}
		else if(selectedV != NULL){
			//クリック座標と最も近い頂点を返却
			CVertex* taisyouV =  CheckSameVertex();

			//同じ座標かチェック
			if (CM.euclid2p(taisyouV->GetX(),taisyouV->GetY(),World_X,World_Y) < 0.03) {
				
				CVertex* preV = NULL;
				//taisyouSに変更しようとしている頂点を含むshapeを入れる
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (nowV->GetX() == taisyouV->GetX(), nowV->GetY() == taisyouV->GetY()) {
							taisyouS = nowS;
							break;
						}
						preV = nowV;
					}
				}

				if (taisyouS->GetCount() > 2) {

					//消そうとしている座標がヘッドの時
					if (taisyouS->GetSHead()->GetX() == taisyouV->GetX() && taisyouS->GetSHead()->GetY() == taisyouV->GetY()) {

					}
					else {
						preV->SetNext(taisyouV->GetNext());

						delete taisyouV;
						
						////すべての頂点で内外判定
						//for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
						//	for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						//		if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
						//			//内外判定に引っかかったら頂点を元に戻す
						//			
						//			//選択頂点の初期化
						//			selectedV == NULL;
						//			break;
						//		}
						//	}
						//}
					}
				}

				
			}
			else {
				taisyouV = NULL;
				tais
			}
		}
	}
}

//マウスクリック関数
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
		selectedV = NULL;
		selectedL = NULL;
		selectedS = NULL;
		//shapeが何もないとき
		if (ShapeHead == NULL) {
			EditFlag = false;
		}
		else {
		//すべての図形の頂点の中である閾値以下の一番近い点を返却
			selectedV = CheckSameVertex();						//頂点、稜線、形状の順に優先度
			if (selectedV == NULL) {	//頂点が選択されていないときに稜線の選択ができる
				LineSelect();
			}
			if (selectedL == NULL && selectedV == NULL)	{	//頂点、稜線が選択されていないときに形状の選択ができる
				CheckShapeSelect();
			}
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
				if (CM.euclid2p(ShapeHead->GetStartVertex()->GetX(), ShapeHead->GetStartVertex()->GetY(), World_X, World_Y) >= 0.03) {	//打った点が最初の頂点と近いときは交差判定をしない
					if (!CheckCrossShape(ShapeHead, World_X,World_Y)) {			//閉じていないとき　交差判定
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
		if (CM.euclid2p((ShapeHead->GetStartVertex()->GetX()), (ShapeHead->GetStartVertex()->GetY()), World_X, World_Y) < 0.03) {	//ヘッドのshapeの始点とクリック座標をユークリッド距離で判別
			if (!CheckCrossShape(ShapeHead, World_X,World_Y)) {					//スタート地点を置いたときに交差判定
				if (ShapeHead->GetCount() > 3) {		//shapeに置かれた頂点の数が３つ以上の時
					if (Scount > 1) {					//shapeのヘッド頂点の次がNULLの時を除く
						ShapeHead->SetStart();			//一時的にvertex_headをスタート地点にする
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
bool CAdminControl::CheckCrossShape(CShape* head, double x, double y)
{
	CShape* now = head;	//今居るshape
	
	for (; now != NULL; now = now->SGetNext()) {	//shapeがNULLになるまで繰り返す

		//今置こうとしている頂点と一つ前においた頂点間のベクトルとnowのshapeに含まれるベクトルを交差判定
		if (now->CheckCrossVertex(head->GetSHead()->GetX(), head->GetSHead()->GetY(), x, y)) {	
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
		for (CVertex* nowV = now->GetSHead()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
			if (CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X,World_Y) < 0.03) {
				if (minEuclid == NULL) {														//minEuclidに何も入っていない場合はそれを入れる
					minEuclid = CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y);
					if (nowV == now->GetSHead() && now->GetClosed()) {
						while (nowV->GetNext() == NULL) {
							nowV = nowV->GetNext();
						}
					}
					MostCloseV = nowV;
				}
				else {
					if (minEuclid > CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y)) {
						minEuclid = CM.euclid2p(nowV->GetX(), nowV->GetY(), World_X, World_Y);
						if (nowV == now->GetSHead() && now->GetClosed()) {
						while (nowV->GetNext() == NULL) {
							nowV = nowV->GetNext();
						}
					}
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

//稜線の選択
void CAdminControl::LineSelect()
{
	CShape* now = ShapeHead;
	CVertex* nowV = NULL;
	CVertex* nextV = NULL;
	CVertex* seL = NULL;
	double minD = NULL;
	//すべての形状を繰り返す
	for (; now != NULL; now = now->SGetNext()) {
		//形状の中の頂点をはじめ(vhead)から繰り返す。
		for (nowV = now->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
			nextV = nowV->GetNext();
			CVect va = CM.CalcVect(nowV, nextV);									//稜線ベクトル
			CVect vb = CM.CalcVect(nowV->GetX(), nowV->GetY(), World_X, World_Y);	//クリックした点とnowVのベクトル
			double d = CM.calcDistancePL(va, vb);		//点と直線の距離の計算
			if (d < 0.02) {
				//点と直線の距離が0.01以下の時
				if (minD == NULL) {
					minD = d;
					CVect p = CM.CalcVect(nowV->GetX(), nowV->GetY(), World_X, World_Y);
					double s = CM.calcNaiseki(p, va) / CM.calcNaiseki(va, va);
					if (s >= 0 && s <= 1) {												//直線の長さの範囲のみ判定 |-|
						seL = nowV;
					}
				}
				else {
					if (minD > d) {
						minD = d;
						CVect p = CM.CalcVect(nowV->GetX(), nowV->GetY(), World_X, World_Y);
						double s = CM.calcNaiseki(p, va) / CM.calcNaiseki(va, va);
						if (s >= 0 && s <= 1) {												//直線の長さの範囲のみ判定 |-|
							seL = nowV;
						}
					}
				}
			}
		}
		selectedL = seL;		//点と直線の距離が一番近い線をselectedとして登録
	}
}


//マウスを動かすと呼ばれる
void CAdminControl::mouseMove(double x, double y, int width, int height)
{
	//デバイスの座標からworld座標系に変換
	Cursor_Pos_X = (x - 0.5) * 2;
	Cursor_Pos_Y = (y - 0.5) * 2;

	//画面の比からworld座標を修正
	if (width > height) {
		Cursor_Pos_X = Cursor_Pos_X * ((double)width / (double)height);
	}
	else {
		Cursor_Pos_Y = Cursor_Pos_Y * ((double)height / (double)width);
	}

	/*if (ShapeHead != NULL) {
		if(ShapeHead)
	}*/
}
