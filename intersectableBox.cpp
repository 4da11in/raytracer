#include "intersectableBox.h"
#include "ray.h"

#include <vector>

intersectableBox::intersectableBox(double x1, double y1, double z1, double x2, double y2, double z2) : e1(x1,y1,z1), e2(x2,y2,z2)
{
}

intersectableBox::intersectableBox(vec e1, vec e2) : e1(e1), e2(e2) {}

double intersectableBox::intersects(ray r) {
	double tNear = -INFINITY;
	double tFar = INFINITY;
	std::vector<vec> intersectionInfo;
	double tNearMax = -INFINITY;
	for (int dim = 0; dim < 3; dim++) {
		if (r.dir[dim] == 0) { // ray is parallel to planes
			if (r.origin[dim] > e2[dim] || r.origin[dim] < e1[dim]) { // if outside bounds of planes, cannot intersect
				return -1;
			}
		} else { // ray not parallel, compute intersections
			double t1 = (e1[dim]-r.origin[dim])/r.dir[dim];
			double t2 = (e2[dim]-r.origin[dim])/r.dir[dim];
			if (t1 > t2) {
				std::swap(t1, t2);
			}
			if (t1 > tNear) {
				tNear = t1;
			}
			if (t2 < tFar) {
				tFar = t2;
			}
			if (tNear > tFar) { // missed
				return -1;
			}
			if (tFar < 0) { // behind
				return -1;
			}
			if (tNear > tNearMax) {
				tNearMax = tNear;
			}
		}
	}	
	return tNearMax;
}