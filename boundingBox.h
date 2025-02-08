#pragma once

#include "vec.h"
#include "intersectableBox.h"

#include <vector>
#include <memory>

class primitive;

class boundingBox : public intersectableBox
{
public:
	boundingBox(vec e1, vec e2);
};

