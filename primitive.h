#pragma once
#include "vec.h"
#include "ray.h"
#include "material.h"
#include "boundingBox.h"
#include <vector>
class primitive
{
public:
	primitive(material mat);
	material mat;
	virtual boundingBox getBoundingBox() = 0;
	virtual std::vector<vec> getIntersection(ray r) = 0;
	int id;
};

