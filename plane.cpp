#include "plane.h"
#include "utilities.h"

plane::plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, material mat) : points({vec(x1, y1, z1), vec(x2, y2, z2) , vec(x3, y3, z3) }), mat(mat)
{

}

std::vector<vec> plane::getIntersection(ray r)
{
	std::vector<vec> intersectionInfo;
	
	// Create normal
	vec a = points[1] - points[0];
	vec b = points[2] - points[0];
	vec normal = normalize(a.cross(b));

	if (normal.dot(r.dir) > 0) {
		// no intersection
		//return intersectionInfo;
	}
	
	double d = -normal.dot(points[0]);
	double intersectionT = -(normal.dot(r.origin)+d)/normal.dot(r.dir);
	vec intersectionPoint = r.at(intersectionT);
	// Alternate method that also works with non-triangular polygons. Not currently functional.
	// find dominant coordinate (max dimension of normal)
	/*int dominantCoordinate = 0;
	if (normal.y > normal.x) {
		dominantCoordinate = 1;
	}
	if (normal.z > normal.y) {
		dominantCoordinate = 2;
	}
	std::vector<vec> projectedPoints;
	vec projectedIntersection(0, 0);
	for (int i = 0; i < 3; i++) {
		if (dominantCoordinate == 0) {
			projectedIntersection = vec(intersectionPoint.y, intersectionPoint.z);
			projectedPoints.push_back(vec(points[i].y, points[i].z) - projectedIntersection);
		}
		if (dominantCoordinate == 1) {
			projectedIntersection = vec(intersectionPoint.x, intersectionPoint.z);
			projectedPoints.push_back(vec(points[i].x, points[i].z) - projectedIntersection);
		}
		if (dominantCoordinate == 2) {
			projectedIntersection = vec(intersectionPoint.x, intersectionPoint.y);
			projectedPoints.push_back(vec(points[i].x, points[i].y) - projectedIntersection);
		}
	}

	double v0 = projectedPoints[0].y;
	int signHolder = 1;
	if (v0 < 0) {
		signHolder = -1;
	}
	int numCrossings = 0;
	for (int i = 0; i < 3; i++) {
		vec thisPoint = projectedPoints[i];
		vec nextPoint(0, 0);
		int nextV;
		if (i == 2) {
			nextPoint = projectedPoints[0];
			nextV = projectedPoints[0].y;
		}
		else {
			nextPoint = projectedPoints[i + 1];
			nextV = projectedPoints[i + 1].y;
		}
		int nextSignHolder = 1;
		if (nextV < 0) {
			nextSignHolder = -1;
		}
		if (signHolder != nextSignHolder) {
			if (thisPoint.x > 0 && nextPoint.x > 0) {
				numCrossings++;
			} else if (thisPoint.x > 0 || nextPoint.x > 0) {
				double ucross = thisPoint.x - thisPoint.y * (nextPoint.x - thisPoint.x) / (nextPoint.y - thisPoint.y);
				if (ucross > 0) {
					numCrossings++;
				}
			}
		}
		signHolder = nextSignHolder;
	}*/
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
