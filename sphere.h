#pragma once
#include "vec.h"
#include "material.h"
#include "primitive.h"
#include "boundingBox.h"

#include <vector>
#include <cmath>

class sphere : public primitive
{
public:
	sphere(double x, double y, double z, double radius, material& mat);

	std::vector<vec> getIntersection(ray r);
	boundingBox getBoundingBox();

	double radius;
	vec center;
	double x;
	double y;
	double z;
	material mat;
};

