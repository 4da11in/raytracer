#pragma once
#include "vec.h"
#include "ray.h"
#include "material.h"
#include <vector>
class plane
{
public:
	plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, material mat);
	material mat;
	std::vector<vec> points;
	std::vector<vec> getIntersection(ray r);
};

