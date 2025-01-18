#include "colorAtRay.h"

vec colorAtRay(ray r, std::vector<sphere> spheres, std::vector<plane> planes, int bounceCount, vec bgColor, directionalLight directionalLight, light ambientLight) {
	vec refColor = bgColor;
	vec outputColor = colorize(bgColor);
	double zbuffer = -999999999;
		
	// Reflections or diffuse for all planes
	for (int i = 0; i < planes.size(); i++) {
		std::vector<vec> intersectionInfo = planes[i].getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			vec normal = intersectionInfo[1];
			ray toLight(intersectionPoint, directionalLight.direction);
			double eta = 0.001;
			vec reflectionOrigin = intersectionPoint + normal * eta;
			vec reflectionDirection = r.dir - normal * 2 * r.dir.dot(normal);
			ray reflectionRay(reflectionOrigin, reflectionDirection);
			// Calculate color
			vec refColor = bgColor;
			if (intersectionPoint.z > zbuffer) {
				outputColor = planes[i].mat.getColor(normal, directionalLight, ambientLight, -r.dir, refColor);
				zbuffer = intersectionPoint.z;
			}
			// NOTE: reflections MUST precede shadows, otherwise shadowed areas may reflect
			// reflections from planes on this plane
			for (int j = 0; j < planes.size(); j++) {
				if (i != j) {
					std::vector<vec> refIntersectionInfo = planes[j].getIntersection(reflectionRay);
					if (bounceCount < 3 && refIntersectionInfo.size() > 0) {
						vec refNormal = refIntersectionInfo[1];
						if (intersectionPoint.z >= zbuffer) {
							vec refColor = colorAtRay(reflectionRay, spheres, planes, bounceCount + 1, bgColor,  directionalLight, ambientLight) * 1.0 / 255;
							outputColor = planes[i].mat.getColor(normal, directionalLight, ambientLight, -r.dir, refColor);
							zbuffer = intersectionPoint.z;
						}
					}
				}
			}
			// reflections from spheres on this plane
			for (int j = 0; j < spheres.size(); j++) {
				if (i != j) {
					std::vector<vec> refIntersectionInfo = spheres[j].getIntersection(reflectionRay);
					if (bounceCount < 3 && refIntersectionInfo.size() > 0) {
						vec refNormal = refIntersectionInfo[1];
						if (intersectionPoint.z >= zbuffer) {
							vec refColor = colorAtRay(reflectionRay, spheres, planes, bounceCount + 1, bgColor,  directionalLight, ambientLight) * 1.0 / 255;
							outputColor = planes[i].mat.getColor(normal, directionalLight, ambientLight, -r.dir, refColor);
							zbuffer = intersectionPoint.z;
						}
					}
				}
			}
			
		}
	}
	
	// Reflections or diffuse for all spheres
	for (int i = 0; i < spheres.size(); i++) {
		std::vector<vec> intersectionInfo = spheres[i].getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			vec normal = intersectionInfo[1];
			ray toLight(intersectionPoint, directionalLight.direction);
			double eta = 0.0001;
			vec reflectionOrigin = intersectionPoint + normal * eta;
			vec reflectionDirection = r.dir - normal * 2 * r.dir.dot(normal);
			ray reflectionRay(reflectionOrigin, reflectionDirection);
			// Calculate color
			if (intersectionPoint.z > zbuffer) {
				outputColor = spheres[i].mat.getColor(normal, directionalLight, ambientLight, -r.dir, refColor);
				zbuffer = intersectionPoint.z;
			}
			// NOTE: reflections MUST precede shadows, otherwise shadowed areas may reflect
			// 
			// reflections from planes on this sphere
			for (int j = 0; j < planes.size(); j++) {
				std::vector<vec> refIntersectionInfo = planes[j].getIntersection(reflectionRay);
				if (bounceCount < 3 && refIntersectionInfo.size() > 0) {
					//outputColor = vec(150, 0, 0);
					vec refNormal = refIntersectionInfo[1];
					if (intersectionPoint.z >= zbuffer) {
						vec refColor = colorAtRay(reflectionRay, spheres, planes, bounceCount + 1, bgColor, directionalLight, ambientLight) * 1.0 / 255;
						outputColor = spheres[i].mat.getColor(normal, directionalLight, ambientLight, -r.dir, refColor);
						zbuffer = intersectionPoint.z;
					}
				}
			}
			// reflections from spheres on this sphere
			for (int j = 0; j < spheres.size(); j++) {
				if (i != j) {
					std::vector<vec> refIntersectionInfo = spheres[j].getIntersection(reflectionRay);
					if (bounceCount < 3 && refIntersectionInfo.size() > 0) {
						vec refNormal = refIntersectionInfo[1];
						if (intersectionPoint.z >= zbuffer) {
							vec refColor = colorAtRay(reflectionRay, spheres, planes, bounceCount + 1, bgColor,  directionalLight, ambientLight) * 1.0 / 255;
							outputColor = spheres[i].mat.getColor(normal, directionalLight, ambientLight, -r.dir, refColor);
							zbuffer = intersectionPoint.z;
						}
					}
				}
			}
			
		}
	}
	// shadows
	for (int i = 0; i < spheres.size(); i++) {
		std::vector<vec> intersectionInfo = spheres[i].getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			ray toLight(intersectionPoint, directionalLight.direction);
			// shadows from planes on this sphere
			for (int j = 0; j < planes.size(); j++) {
				if (planes[j].getIntersection(toLight).size() > 0) {
					if (intersectionPoint.z >= zbuffer) {
						// We've already tried all diffuse/specular options, so if this is in shadow, we don't care *which* shadow has the biggest z-index, so just return black
						return vec(0, 0, 0);
						zbuffer = intersectionPoint.z;
					}
				}
			}
			// shadows from spheres on this sphere
			for (int j = 0; j < spheres.size(); j++) {
				if (i != j) {
					if (spheres[j].getIntersection(toLight).size() > 0) {
						if (intersectionPoint.z >= zbuffer) {
							return vec(0, 0, 0);
							zbuffer = intersectionPoint.z;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < planes.size(); i++) {
		std::vector<vec> intersectionInfo = planes[i].getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			ray toLight(intersectionPoint, directionalLight.direction);
			// shadows from planes on this plane
			for (int j = 0; j < planes.size(); j++) {
				if (i != j) {
					if (planes[j].getIntersection(toLight).size() > 0) {
						// In shadow
						if (intersectionPoint.z >= zbuffer) {
							return vec(0, 0, 0);
							zbuffer = intersectionPoint.z;
						}
					}
				}
			}
			// shadows from spheres on this plane
			for (int j = 0; j < spheres.size(); j++) {
				if (spheres[j].getIntersection(toLight).size() > 0) {
					if (intersectionPoint.z >= zbuffer) {
						return vec(0, 0, 0);
						zbuffer = intersectionPoint.z;
					}
				}
			}
		}
	}
	return outputColor;
}