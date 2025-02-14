#pragma once

#include "vec.h"
#include "primitive.h"
#include "intersectableBox.h"

class boundingVolumeHierarchy : public intersectableBox
{
public:
	boundingVolumeHierarchy(vec e1, vec e2);

	void medianSplit(int subdivLevel, int minChildren);
	bool checkOverlaps(intersectableBox a, intersectableBox b);
	std::vector<std::shared_ptr<boundingVolumeHierarchy>> boundingChildren;
	std::vector<std::shared_ptr<primitive>> primChildren;
	friend std::ostream& operator<<(std::ostream& os, boundingVolumeHierarchy& bv);
};

