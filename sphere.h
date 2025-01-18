#pragma once
#include "vec.h"
#include "material.h"
#include "primitive.h"
#include <vector>

class sphere
{
public:
	sphere(double x, double y, double z, double radius, material& mat);

	std::vector<vec> getIntersection(ray r);
	double radius;
	vec center;
	double x;
	double y;
	double z;
	material mat;
};

