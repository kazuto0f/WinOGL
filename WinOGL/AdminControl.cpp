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
	basePoint = NULL;
}

CAdminControl::~CAdminControl()
{
	FreeShape();
	if (basePoint != NULL) {
		delete basePoint;
	}
}



void CAdminControl::OnDraw() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT */);

	CShape* nowS = ShapeHead;
	CShape* preS = ShapeHead;

	while (nowS != NULL) {
		nowS->DrawShape(1.0, 1.0, 1.0);
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
		if (basePoint != NULL) {
			glBegin(GL_POINTS);
			glColor3f(1.0, 0.0, 1.0);
			glVertex2f(basePoint->GetX(), basePoint->GetY());
			glEnd();
		}
	}
	//else {	//editFlagがオフの時選択を解除
	//	selectedV = NULL;
	//	selectedS = NULL;
	//	selectedL = NULL;
	//	/*if (ShapeHead != NULL || selectedS->GetBasePoint() != NULL) {
	//		delete selectedS->GetBasePoint();
	//	}*/
//}
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

		//判定を入れる
		bool ret = false;

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

					//内外判定
					//shapeが閉じられていないときはshapeheadの次から判定
					if (!ShapeHead->GetClosed()) {
						//すべての頂点で内外判定
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
							}
							if (ret)break;
						}

						//交差判定
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {

							//選択された頂点の次の頂点がNULLでないときの交差判定
							if (selectedV->GetNext() != NULL) {
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), selectedV->GetNext()->GetX(), selectedV->GetNext()->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
							}
							//選択された頂点の次の頂点がNULLのときの交差判定
							else {
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), taisyouS->GetSHead()->GetNext()->GetX(), taisyouS->GetSHead()->GetNext()->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
							}
						}
					}
					//shapeが閉じられているとき
					/*else if (checkNaigai(taisyouS, World_X, World_Y)) {
						ret = true;
					}*/
					else
					{
						//すべての頂点で内外判定
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
							}
							if (ret)break;
						}

						//交差判定
						for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {

							//選択された頂点の次の頂点がNULL出ないときの交差判定
							if (selectedV->GetNext() != NULL) {
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), selectedV->GetNext()->GetX(), selectedV->GetNext()->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
							}
							//選択された頂点の次の頂点がNULLのときの交差判定
							else {
								if (nowS->CheckCrossVertex(preSV->GetX(), preSV->GetY(), selectedV->GetX(), selectedV->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
								if (nowS->CheckCrossVertex(selectedV->GetX(), selectedV->GetY(), taisyouS->GetSHead()->GetNext()->GetX(), taisyouS->GetSHead()->GetNext()->GetY())) {
									//交差判定に引っかかったらretをtrue
									ret = true;
									break;
								}
							}
						}
					}

					if (ret) {
						//選択された頂点の座標をもとに戻す
						selectedV->SetXY(temp_x, temp_y);
						if (selectedHead != NULL)
							selectedHead->SetXY(temp_x, temp_y);

						//選択頂点の初期化
						selectedV == NULL;
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
		//戻すかを判別するフラグ
		bool ret = false;

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
			//taisyouSに変更しようとしている頂点を含むshapeを入れる
			for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
				for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
					if (nowV->GetX() == selectedL->GetX(), nowV->GetY() == selectedL->GetY()) {
						taisyouS = nowS;
						break;
					}
				}
				if (taisyouS != NULL)	break;
			}

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

				taisyouS->SetCount(taisyouS->GetCount() + 1);

				selectedL = NULL;
			}
		}
		else if (selectedV != NULL) {
			CVertex* preV = NULL;
			CVertex* PreEnd = NULL;

			//形状が閉じられていないとき
			if (!ShapeHead->GetClosed()) {
				//taisyouSに変更しようとしている頂点を含むshapeを入れる
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					preV = nowS->GetSHead();
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (nowV->GetX() == selectedV->GetX(), nowV->GetY() == selectedV->GetY()) {
							taisyouS = nowS;
							break;
						}
						preV = nowV;
					}
					if (taisyouS != NULL)	break;
				}

				//形状の最終の一つ前をPreEndに入れる
				//if (taisyouS != ShapeHead) {
					for (CVertex* nowV = taisyouS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
						if (nowV->GetNext()->GetNext() == NULL) {
							PreEnd = nowV;
						}
					}
				//}
			}
			else {
				//taisyouSに変更しようとしている頂点を含むshapeを入れる
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					preV = nowS->GetSHead();
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (nowV->GetX() == selectedV->GetX(), nowV->GetY() == selectedV->GetY()) {
							taisyouS = nowS;
							break;
						}
						preV = nowV;
					}
					if (taisyouS != NULL)	break;
				}

				//形状の最終の一つ前をPreEndに入れる
				for (CVertex* nowV = taisyouS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
					if (nowV->GetNext()->GetNext() == NULL) {
						PreEnd = nowV;
					}
				}
			}

			bool delhead = false;


			if (taisyouS->GetCount() > 4) {
				//消そうとしている座標がヘッドの時
				if (taisyouS->GetSHead()->GetX() == selectedV->GetX() && taisyouS->GetSHead()->GetY() == selectedV->GetY()) {

					delhead = true;

					//頂点ヘッド,終点の一時保存
					CVertex* tempHead = taisyouS->GetSHead();
					CVertex* tempEnd = PreEnd->GetNext();

					//形状のヘッドを一つずらす
					taisyouS->SetShead(taisyouS->GetSHead()->GetNext());

					//（頂点挿入）新しいヘッドに新しい終点を生成しつなげる
					CVertex* New = new CVertex();
					New->SetXY(taisyouS->GetSHead()->GetX(), taisyouS->GetSHead()->GetY());
					PreEnd->SetNext(New);

					//形状の頂点数の再設定
					taisyouS->SetCount(taisyouS->GetCount() - 1);

					//if (!ShapeHead->GetClosed()) {
					//	//内外判定
					//	for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
					//		for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
					//			//内外判定がtrueの時
					//			if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
					//				//内外判定に入ったフラグ
					//				ret = true;
					//			}
					//		}
					//	}

					//	//内外判定がfalseのとき自己交差判定
					//	if (!ret) {
					//		if (taisyouS->CheckCrossVertex(PreEnd->GetX(), PreEnd->GetY(), preV->GetX(), preV->GetY()) == true)
					//			ret = true;
					//	}

					//	//削除系の処理
					//	if (ret == true) {
					//		//頂点の数を戻す
					//		taisyouS->SetCount(taisyouS->GetCount() + 1);
					//		//終点を戻す
					//		PreEnd->SetNext(tempEnd);
					//		//ヘッドを戻す
					//		taisyouS->SetShead(tempHead);
					//		//追加した点を削除
					//		delete New;
					//	}
					//	else {
					//		delete tempHead;
					//		delete tempEnd;
					//	}
					//}
					//else {

					//内外判定
					if (!ShapeHead->GetClosed()) {
						for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								//内外判定がtrueの時
								if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
									//内外判定に入ったフラグ
									ret = true;
								}
							}
						}
					}
					else {
						for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								//内外判定がtrueの時
								if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
									//内外判定に入ったフラグ
									ret = true;
								}
							}
						}
					}


					//自己交差判定

					if (taisyouS->CheckCrossVertex(PreEnd->GetX(), PreEnd->GetY(), preV->GetX(), preV->GetY()) == true)
						ret = true;

					//交差判定
					for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
						for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
							for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
								if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
									ret = true;
									break;
								}
							}
							if (ret)	break;
						}
						if (ret)	break;
					}


					//削除系の処理
					if (ret == true) {
						//頂点の数を戻す
						taisyouS->SetCount(taisyouS->GetCount() + 1);
						//終点を戻す
						PreEnd->SetNext(tempEnd);
						//ヘッドを戻す
						taisyouS->SetShead(tempHead);
						//追加した点を削除
						delete New;
					}
					else {
						delete tempHead;
						delete tempEnd;
					}
					//}
				}
				else {

					//頂点の挿入
					preV->SetNext(selectedV->GetNext());
					//頂点数の再設定
					taisyouS->SetCount(taisyouS->GetCount() - 1);

					//内外判定
					if (ShapeHead->GetClosed()) {
						for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
									//内外判定がtrueの時

									//頂点の数を戻す
									taisyouS->SetCount(taisyouS->GetCount() + 1);
									//選択した頂点をリストに戻す
									preV->SetNext(selectedV);
									//内外判定に入ったフラグ
									ret = true;
								}
							}
						}
					}
					else {
						for (CShape* nowS = ShapeHead; nowS->SGetNext() != NULL; nowS = nowS->SGetNext()) {
							for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
								if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
									//内外判定がtrueの時

									//頂点の数を戻す
									taisyouS->SetCount(taisyouS->GetCount() + 1);
									//選択した頂点をリストに戻す
									preV->SetNext(selectedV);
									//内外判定に入ったフラグ
									ret = true;
								}
							}
						}
					}

					//自己交差判定

					if (taisyouS->CheckCrossVertex(preV->GetX(), preV->GetY(), preV->GetNext()->GetX(), preV->GetNext()->GetY()) == true)
						ret = true;

					//交差判定
					for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
						for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
							for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
								if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
									ret = true;
									break;
								}
							}
							if (ret)	break;
						}
						if (ret)	break;
					}

					//削除系の処理
					if (!ret) {
						delete selectedV;
					}
					else {
						preV->SetNext(selectedV);
						taisyouS->SetCount(taisyouS->GetCount() + 1);
					}
				}
			}
		}
		selectedV = NULL;
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
		/*if (basePoint != NULL) {
			delete basePoint;
			basePoint = NULL;
		}*/
		
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
			if (selectedL == NULL && selectedV == NULL) {	//頂点、稜線が選択されていないときに形状の選択ができる
				CheckShapeSelect();
			}
		}
	}
	else{
		//基点のリセット
		delete basePoint;
		basePoint = NULL;

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
					ShapeHead->SSetFront(New);
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

// マウスを動かすと呼ばれる(左クリックが押された状態)
void CAdminControl::mouseMove(double x, double y, int width, int height)
{
	//内外または交差判定を入れる
	bool ret = false;
	//形状が選択されている時
	if (selectedS != NULL) {

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

		//基点がある場合
		if (basePoint != NULL) {
			rotateShape(Cursor_Pos_X, Pre_Cursor_Pos_Y);
		}
		//基点がない場合
		else {
			//図形の中心座標を計算
			double tx = selectedS->GetSHead()->GetX();
			double ty = selectedS->GetSHead()->GetY();
			for (CVertex* nowV = selectedS->GetSHead()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
				tx += nowV->GetX();
				ty += nowV->GetY();
			}
			tx = tx / selectedS->GetCount();
			ty = ty / selectedS->GetCount();



			//座標に中心座標とマウス座標の差を足す
			for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
				nowV->SetX(nowV->GetX() + (Cursor_Pos_X - tx));
				nowV->SetY(nowV->GetY() + (Cursor_Pos_Y - ty));
			}

			if (!ShapeHead->GetClosed()) {
				//内外判定
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
			}
			else {
				//内外判定
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
			}

			//交差判定
			for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
				for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
					for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
						if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
				if (ret)	break;
			}

			if (ret) {
				for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
					nowV->SetX(nowV->GetX() - (Cursor_Pos_X - tx));
					nowV->SetY(nowV->GetY() - (Cursor_Pos_Y - ty));
				}
			}

		}
		Pre_Cursor_Pos_X = Cursor_Pos_X;
		Pre_Cursor_Pos_Y = Cursor_Pos_Y;
	}
}

//拡大関数
void CAdminControl::mouseWheel(short wheel)
{
	bool ret = false;

	if (selectedS != NULL) {
		if (basePoint != NULL) {
			for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
				if (wheel > 0) {
					CVect V = CM.CalcVect(basePoint,nowV);
					nowV->SetX(1.03 * ((nowV->GetX()) - (basePoint->GetX())) + (basePoint->GetX()));
					nowV->SetY(1.03 * ((nowV->GetY()) - (basePoint->GetY())) + (basePoint->GetY()));
				}
				else {
					CVect V = CM.CalcVect(basePoint, nowV);
					nowV->SetX(0.97087379 * ((nowV->GetX()) - (basePoint->GetX())) + (basePoint->GetX()));
					nowV->SetY(0.97087379 * ((nowV->GetY()) - (basePoint->GetY())) + (basePoint->GetY()));
				}
			}

			if (!ShapeHead->GetClosed()) {
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
			}
			else {

				// 内外判定
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
			}

			//交差判定
			for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
				for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
					for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
						if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
				if (ret)	break;
			}

			//ret が trueのとき元に戻す
			if (ret) {
				for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
					if (wheel > 0) {
						CVect V = CM.CalcVect(basePoint, nowV);
						nowV->SetX(0.97087379 * ((nowV->GetX()) - (basePoint->GetX())) + (basePoint->GetX()));
						nowV->SetY(0.97087379 * ((nowV->GetY()) - (basePoint->GetY())) + (basePoint->GetY()));
					}
					else {
						CVect V = CM.CalcVect(basePoint, nowV);
						nowV->SetX(1.03 * ((nowV->GetX()) - (basePoint->GetX())) + (basePoint->GetX()));
						nowV->SetY(1.03 * ((nowV->GetY()) - (basePoint->GetY())) + (basePoint->GetY()));
					}
				}
			}
		}
	}
}

//回転関数
void CAdminControl::rotateShape(double x, double y)
{
	bool ret = false;

	if (selectedS != NULL) {
		if (basePoint != NULL) {

			CVect bm = CM.CalcVect(basePoint->GetX(), basePoint->GetY(), x, y);
			CVect bv = CM.CalcVect(basePoint, selectedS->GetSHead());
			double rad = CM.calcAngle(bv, bm);

			for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {

				double x2 = ((nowV->GetX() - basePoint->GetX()) * cos(rad)) - ((nowV->GetY() - basePoint->GetY()) * sin(rad)) + basePoint->GetX();
				double y2 = ((nowV->GetX() - basePoint->GetX()) * sin(rad)) + ((nowV->GetY() - basePoint->GetY()) * cos(rad)) + basePoint->GetY();

				nowV->SetXY(x2, y2);
			}
			if (!ShapeHead->GetClosed()) {
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai2(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}

					if (ret)	break;
				}
			}
			else {

				// 内外判定
				for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
					for (CVertex* nowV = nowS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {
						if (checkNaigai(nowS, nowV->GetX(), nowV->GetY())) {
							ret = true;
							break;
						}
					}

					if (ret)	break;
				}
			}

			//交差判定
			for (CShape* nowS = ShapeHead; nowS != NULL; nowS = nowS->SGetNext()) {
				for (CVertex* nowV = nowS->GetSHead(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
					for (CShape* checkS = ShapeHead; checkS != NULL; checkS = checkS->SGetNext()) {
						if (checkS->CheckCrossVertex(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY())) {
							ret = true;
							break;
						}
					}
					if (ret)	break;
				}
				if (ret)	break;
			}

			//判定に引っかかったら元に戻す
			if (ret) {
				for (CVertex* nowV = selectedS->GetSHead(); nowV != NULL; nowV = nowV->GetNext()) {

					double x2 = ((nowV->GetX() - basePoint->GetX()) * cos(-rad)) - ((nowV->GetY() - basePoint->GetY()) * sin(-rad)) + basePoint->GetX();
					double y2 = ((nowV->GetX() - basePoint->GetX()) * sin(-rad)) + ((nowV->GetY() - basePoint->GetY()) * cos(-rad)) + basePoint->GetY();

					nowV->SetXY(x2, y2);
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
						ShapeHead->SetStartPos();			//一時的にvertex_headをスタート地点にする
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

//形状が閉じられていないときの内外判定
bool CAdminControl::checkNaigai2(CShape* TaisyouS, double x, double y)
{
	CShape* now;	//今居るshape
	now = ShapeHead;
	for (; now != NULL; now = now->SGetNext()) {	//shapeがNULLになるまで繰り返す
		if (TaisyouS == now)						//対象の頂点が含まれるshapeを省く
			continue;
		if (ShapeHead == now)
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

//選択のリセット
void CAdminControl::resetSelect()
{	
	if (basePoint != NULL) {
		delete basePoint;
	}
	basePoint = NULL;
	selectedV = NULL;
	selectedL = NULL;
	selectedS = NULL;
}

//基点の設定
void CAdminControl::SetBasePoint(double x, double y, int width, int height)
{
	//デバイスの座標からworld座標系に変換
	x = (x - 0.5) * 2;
	y = (y - 0.5) * 2;

	//画面の比からworld座標を修正
	if (width > height) {
		x = x * ((double)width / (double)height);
	}
	else {
		y = y * ((double)height / (double)width);
	}

	//basepointに何か入っているとき
	if (basePoint != NULL) {
		//クリック座標がbasepointと近いとき
		if (CM.euclid2p(basePoint->GetX(), basePoint->GetY(), x, y) < 0.05) {
			delete basePoint;
			basePoint = NULL;
		}
		//遠いときは移動させる
		else {
			basePoint->SetXY(x, y);
		}
	}
	//NULのときは新しく生成
	else {
		CVertex* New = new CVertex();
		New->SetXY(x, y);
		basePoint = New;
	}
}

//基点の返却
bool CAdminControl::GetBasePoint() {
	if (basePoint != NULL) {
		return true;
	}
	else {
		return false;
	}
}



