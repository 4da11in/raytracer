#include "boundingVolumeHierarchy.h"

boundingVolumeHierarchy::boundingVolumeHierarchy(vec e1, vec e2) : intersectableBox(e1, e2) {}

bool checkOverlaps(intersectableBox a, intersectableBox b) {
	// std::cout << "checked bounds: " << a.e1 << a.e2 << b.e1 << b.e2 << '\n';
	if (b.e1.x > a.e2.x || a.e1.x > b.e2.x ||
		b.e1.y > a.e2.y || a.e1.y > b.e2.y ||
		b.e1.z > a.e2.z || a.e1.z > b.e2.z) {
		return false;
	}
	return true;
}

void boundingVolumeHierarchy::medianSplit(int subdivLevel, int minChildren)
{
	vec measurements = e2-e1;
	double max_dim_size = measurements[0];
	int max_dim_index = 0;
	if (measurements[1] > max_dim_size) {
		max_dim_index = 1;
		max_dim_size = measurements[1];
	}
	if (measurements[2] > max_dim_size) {
		max_dim_index = 2;
		max_dim_size = measurements[2];
	}

	boundingVolumeHierarchy a(e1, e2);
	boundingVolumeHierarchy b(e1, e2);

	double split = (e1[max_dim_index] + e2[max_dim_index])/2;
	a.e2[max_dim_index] = split;
	b.e1[max_dim_index] = split;

	// std::cout << "a.e: " << a.e1[max_dim_index] << " " << a.e2[max_dim_index] << '\n';
	// std::cout << "b.e: " << b.e1[max_dim_index] << " " << b.e2[max_dim_index] << '\n';
	for (std::shared_ptr<primitive> child : primChildren) {
		boundingBox bbox = child->getBoundingBox();
		
		if (checkOverlaps(bbox, a)) {
			a.primChildren.push_back(child);
			// std::cout << "into a: " << child->id << "->" << bbox.e1[max_dim_index] << " " << bbox.e2[max_dim_index] << '\n';
		}
		if (checkOverlaps(bbox, b)) {
			b.primChildren.push_back(child);
			// std::cout << "into b: " << child->id << "->" << bbox.e1[max_dim_index] << " " << bbox.e2[max_dim_index] << '\n';
		}
		if (checkOverlaps(bbox, a) && checkOverlaps(bbox, b)) {
			// std::cout << "ab: " << bbox.e1[max_dim_index] << " " << bbox.e2[max_dim_index] << '\n';
		}
		if (!(checkOverlaps(bbox, a) || checkOverlaps(bbox, b))) {
			std::cout << "dropping: " << child->id << bbox.e1[max_dim_index] << " " << bbox.e2[max_dim_index] << '\n';
		}
	}
	// std::cout << "split " << primChildren.size() << " along axis " << max_dim_index << " into " << a.primChildren.size() << ", " << b.primChildren.size() << '\n';
	primChildren.clear();
	if (subdivLevel > 0 && a.primChildren.size() > minChildren && b.primChildren.size() > minChildren) {
		a.medianSplit(subdivLevel-1, minChildren);
		b.medianSplit(subdivLevel-1, minChildren);
	} else {
		// std::cout << "Final leaf sizes: " << a.primChildren.size() << ' ' << b.primChildren.size() << '\n';
		// std::cout << "a children: " << '\n';
		// for (std::shared_ptr<primitive> child : a.primChildren) {
		// 	std::cout << child->id << ' ';
		// }
		// std::cout << "\nb children: " << '\n';
		// for (std::shared_ptr<primitive> child : b.primChildren) {
		// 	std::cout << child->id << ' ';
		// }
	}
	// boundingChildren.clear();
	boundingChildren.push_back(std::make_shared<boundingVolumeHierarchy>(a));
	boundingChildren.push_back(std::make_shared<boundingVolumeHierarchy>(b));
}

std::ostream &operator<<(std::ostream &os, boundingVolumeHierarchy &bv)
{
	os << bv.boundingChildren.size() << ":" << bv.primChildren.size() << " ";
	for (std::shared_ptr<boundingVolumeHierarchy> b : bv.boundingChildren) {
		os << *b;
	}
	return os;
}
