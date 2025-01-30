#pragma once
#include "light.h"
#include "plane.h"
class areaLight : public light, public plane
{
	public:
		areaLight(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, vec color, double intensity);
};

