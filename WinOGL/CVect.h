#pragma once
#include "CVertex.h"
class CVect
{
public:
	CVect();
	CVect(double x, double y);
	~CVect();

private:
	double vect_x;
	double vect_y;

public:
	double GetXVec();
	double GetYVec();

	//ƒxƒNƒgƒ‹‚ÌŠ|‚¯ŽZ
	void vectTimes(double t);

	void setX(double x);
	void setY(double y);
};

