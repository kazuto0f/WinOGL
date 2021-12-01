#include "pch.h"
#include "CVect.h"
CVect::CVect()
{
	vect_x = 0;
	vect_y = 0;
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

//ƒxƒNƒgƒ‹‚ÌŠ|‚¯ŽZ
void CVect::vectTimes(double t)
{
	vect_x = vect_x * t;
	vect_y = vect_y * t;
}

void CVect::setX(double x)
{
	vect_x = x;
}

void CVect::setY(double y)
{
	vect_y = y;
}

