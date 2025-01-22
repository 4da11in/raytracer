#include "colorAtRay.h"

vec colorAtRay(ray r, std::vector<std::shared_ptr<primitive>> objects, int bounceCount, vec bgColor, directionalLight directionalLight, light ambientLight) {
	vec refColor = bgColor;
	vec outputColor = colorize(bgColor);
	double zbuffer = -999999999;

	// std::vector<std::unique_ptr<primitive>> objects = {};
	// for (plane p : planes) {
	// 	objects.push_back(std::make_unique<plane>(p));
	// }
	// for (sphere s : spheres) {
	// 	objects.push_back(std::make_unique<sphere>(s));
	// }
	
	// Reflections or diffuse for all spheres
	for (int i = 0; i < objects.size(); i++) {
		std::vector<vec> intersectionInfo = objects[i]->getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			vec normal = intersectionInfo[1];
			ray toLight(intersectionPoint, directionalLight.direction);

			// Diffuse color
			if (intersectionPoint.z > zbuffer) {
				outputColor = objects[i]->mat.getColor(normal, directionalLight, ambientLight, -r.dir, refColor);
				zbuffer = intersectionPoint.z;
			}
			// NOTE: reflections MUST precede shadows, otherwise shadowed areas may reflect
			// 
			// reflections from planes on this sphere
			double eta = 0.0001;
			vec reflectionOrigin = intersectionPoint + normal * eta;
			vec reflectionDirection = r.dir - normal * 2 * r.dir.dot(normal);
			ray reflectionRay(reflectionOrigin, reflectionDirection);
			for (int j = 0; j < objects.size(); j++) {
				if (i != j) {
					std::vector<vec> refIntersectionInfo = objects[j]->getIntersection(reflectionRay);
					if (bounceCount < 3 && refIntersectionInfo.size() > 0) {
						//outputColor = vec(150, 0, 0);
						vec refNormal = refIntersectionInfo[1];
						if (intersectionPoint.z >= zbuffer) {
							vec refColor = colorAtRay(reflectionRay, objects, bounceCount + 1, bgColor, directionalLight, ambientLight) * 1.0 / 255;
							outputColor = objects[i]->mat.getColor(normal, directionalLight, ambientLight, -r.dir, refColor);
							zbuffer = intersectionPoint.z;
						}
					}
				}
				
			}			
		}
	}
	// shadows
	for (int i = 0; i < objects.size(); i++) {
		std::vector<vec> intersectionInfo = objects[i]->getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			ray toLight(intersectionPoint, directionalLight.direction);
			for (int j = 0; j < objects.size(); j++) {
				if (i != j) {
					if (objects[j]->getIntersection(toLight).size() > 0) {
						// In shadow
						if (intersectionPoint.z >= zbuffer) {
							return vec(0, 0, 0);
						}
					}
				}
			}
		}
	}
	return outputColor;
}