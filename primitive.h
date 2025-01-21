#pragma once
#include "vec.h"
#include "ray.h"
#include "material.h"
#include <vector>
class primitive
{
public:
	virtual std::vector<vec> getIntersection(ray r) = 0;
	primitive(material mat);
	material mat;
};

