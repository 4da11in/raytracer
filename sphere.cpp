#include "sphere.h"

sphere::sphere(double x, double y, double z, double radius, material& mat) :
	x(x), y(y), z(z), radius(radius), mat(mat), center(vec(x, y, z)), primitive(mat) {}

std::vector<vec> sphere::getIntersection(ray r) {
	std::vector<vec> intersectionInfo;
	vec sphereOffset = r.origin - center;
	double a = dot(r.dir, r.dir);
	double b = 2.0 * dot(r.dir, sphereOffset);
	double c = dot(sphereOffset, sphereOffset) - radius * radius;
	double discriminant = b * b - 4 * a * c;

	double intersectionT = (-b - sqrt(discriminant)) / (2.0 * a);
	vec intersectionPoint = r.at(intersectionT);
	vec normal = normalize(intersectionPoint - center);

	if (discriminant >= 0) {
		if (intersectionT > 0) {
			intersectionInfo.push_back(intersectionPoint);
			intersectionInfo.push_back(normal);
		}
	}
	return intersectionInfo;
}