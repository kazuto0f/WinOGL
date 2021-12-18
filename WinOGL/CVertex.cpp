#include "pch.h"
#include "CVertex.h"

CVertex::CVertex()
{
	x = NULL;
	y = NULL;
	next_vertex = NULL;
	front_vertex = NULL;
}

CVertex::CVertex(double new_x,double new_y, CVertex* new_next, CVertex* new_front)
{
	SetXY(new_x, new_y);
	SetNext(new_next);
	SetFront(new_front);
}

CVertex::~CVertex()
{

}

//　頂点のX座標を書き込む
void CVertex::SetX(double new_x)
{
	x = new_x;
}

//　頂点のY座標を書き込む
void CVertex::SetY(double new_y)
{
	y = new_y;
}

//　頂点のX・Y座標の両方を書き込む
void CVertex::SetXY(double new_x, double new_y)
{
	x = new_x;
	y = new_y;
}

//　頂点のX座標を読み込む込む
double CVertex::GetX()
{
	return x;
}

//　頂点のY座標を読み込む
double CVertex::GetY()
{
	return y;
}

//　頂点のX・Y座標の両方を読み込む
void CVertex::GetXY(double* get_x, double* get_y)
{
	get_x = &x;
	get_y = &y;
}

//　次の頂点リストを指すポインタを書き込む
void CVertex::SetNext(CVertex* new_next)
{
	next_vertex = new_next;
}

//前の頂点のリストを示すポインタを書き込む
void CVertex::SetFront(CVertex* new_front)
{
}

//　次の頂点リストを指すポインタを読み込む
CVertex* CVertex::GetNext()
{
	return next_vertex;
}