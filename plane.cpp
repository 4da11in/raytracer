#include "plane.h"
#include "utilities.h"

plane::plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) :
	points({vec(x1, y1, z1), vec(x2, y2, z2) , vec(x3, y3, z3) }) {}

std::vector<vec> plane::getIntersection(ray r)
{
	std::vector<vec> intersectionInfo;
	
	// Create normal
	vec a = points[1] - points[0];
	vec b = points[2] - points[0];
	vec normal = normalize(a.cross(b));
	
	double d = -normal.dot(points[0]);
	double intersectionT = -(normal.dot(r.origin)+d)/normal.dot(r.dir);
	vec intersectionPoint = r.at(intersectionT);
	
	vec edge0 = points[1] - points[0];
	vec edge1 = points[2] - points[1];
	vec edge2 = points[0] - points[2];
	vec interVec0 = intersectionPoint - points[0];
	vec interVec1 = intersectionPoint - points[1];
	vec interVec2 = intersectionPoint - points[2];

	if (intersectionT > 0 &&
		dot(normal, edge0.cross(interVec0)) > 0 &&
		dot(normal, edge1.cross(interVec1)) > 0 &&
		dot(normal, edge2.cross(interVec2)) > 0) {
		intersectionInfo.push_back(intersectionPoint);
		intersectionInfo.push_back(normal);
	}
	return intersectionInfo;
}
