#pragma once

#include "vec.h"
#include "ray.h"

class primitive;

class intersectableBox
{
public:
	intersectableBox(double x1, double y1, double z1, double x2, double y2, double z2);
	intersectableBox(vec e1, vec e2);

	double intersects(ray r);

	vec e1;
	vec e2;
};

