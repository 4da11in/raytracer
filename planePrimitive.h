#pragma once
#include "vec.h"
#include "ray.h"
#include "material.h"
#include "primitive.h"
#include "plane.h"

#include <vector>
class planePrimitive : public primitive, public plane
{
public:
	planePrimitive(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, material mat);
	std::vector<vec> getIntersection(ray r);
};

