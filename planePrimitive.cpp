#include "planePrimitive.h"
#include "utilities.h"

planePrimitive::planePrimitive(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, material mat) :
	plane(x1, y1, z1, x2, y2, z2, x3, y3, z3), primitive(mat) {}
// multiple inheritance used here to avoid duplicating getIntersection code for a plane (which is also used in areaLight) while still allowing
// the flexibility of having sphere and planePrimitive both inherit from the primitive class (so both can be rendered in the same loop)
std::vector<vec> planePrimitive::getIntersection(ray r) {
	return plane::getIntersection(r);
}