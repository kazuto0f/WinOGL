#include "pch.h"
#include "CVect.h"
CVect::CVect()
{
}

CVect::CVect(double x, double y)
{
	vect_x = x;
	vect_y = y;
}

CVect::~CVect()
{
}

double CVect::GetXVec()
{
	return vect_x;
}

double CVect::GetYVec()
{
	return vect_y;
}

