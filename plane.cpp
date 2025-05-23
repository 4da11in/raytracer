#include "plane.h"

plane::plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, material mat) :
	points({vec(x1, y1, z1), vec(x2, y2, z2) , vec(x3, y3, z3) }), primitive(mat) {}

std::vector<vec> plane::getIntersection(ray r)
{
	std::vector<vec> intersectionInfo;
	
	// Create normal
	vec a = points[1] - points[0];
	vec b = points[2] - points[0];
	vec normal = normalize(cross(a, b));
	
	double d = -dot(normal, points[0]);
	double intersectionT = -(dot(normal, r.origin)+d)/dot(normal, r.dir);
	vec intersectionPoint = r.at(intersectionT);
	
	vec edge0 = points[1] - points[0];
	vec edge1 = points[2] - points[1];
	vec edge2 = points[0] - points[2];
	vec interVec0 = intersectionPoint - points[0];
	vec interVec1 = intersectionPoint - points[1];
	vec interVec2 = intersectionPoint - points[2];

	if (intersectionT > 0 &&
		dot(normal, cross(edge0, interVec0)) > 0 &&
		dot(normal, cross(edge1, interVec1)) > 0 &&
		dot(normal, cross(edge2, interVec2)) > 0) {
		intersectionInfo.push_back(intersectionPoint);
		intersectionInfo.push_back(normal);
	}
	return intersectionInfo;
}

boundingBox plane::getBoundingBox()
{
	vec e1 = points[0];
	vec e2 = points[0];
	for (int i = 0; i < points.size(); i++) {
		for (int dim = 0; dim < 3; dim++) {
			if (points[i][dim] < e1[dim]) {
				e1[dim] = points[i][dim];
			}
			if (points[i][dim] > e2[dim]) {
				e2[dim] = points[i][dim];
			}
		}
	}
    return boundingBox(e1, e2);
}
