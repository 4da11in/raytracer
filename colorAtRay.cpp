#include "colorAtRay.h"

vec colorAtRay(ray r, std::vector<std::shared_ptr<primitive>> objects, int bounceCount, vec bgColor, directionalLight directionalLight, light ambientLight, pointLight pointLight) {
	vec refColor = bgColor;
	vec outputColor = colorize(bgColor);
	double zbuffer = -999999999;
	
	// Reflections and diffuse
	for (int i = 0; i < objects.size(); i++) {
		std::vector<vec> intersectionInfo = objects[i]->getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			vec normal = intersectionInfo[1];
			ray toLight(intersectionPoint, directionalLight.direction);
			vec ptLightDir = pointLight.location - intersectionPoint;
			// reflection ray
			double eta = 0.0001;
			vec reflectionOrigin = intersectionPoint + normal * eta;
			vec reflectionDirection = r.dir - normal * 2 * r.dir.dot(normal);
			ray reflectionRay(reflectionOrigin, reflectionDirection);
			// refraction ray
			double ior = objects[i]->mat.ior;
			vec refractionPerp = (r.dir)*1/ior;
			vec refractionParallel = -normal*sqrt(1-refractionPerp.length()*refractionPerp.length());
			vec refractionDirection = refractionParallel + refractionPerp;
			ray refractionRay(intersectionPoint, refractionDirection);
			
			if (bounceCount < 3) {
				if (intersectionPoint.z >= zbuffer) {
					vec reflectionColor = colorAtRay(reflectionRay, objects, bounceCount + 1, bgColor, directionalLight, ambientLight, pointLight) * 1.0 / 255;
					vec refractionColor = colorAtRay(refractionRay, objects, bounceCount + 1, bgColor, directionalLight, ambientLight, pointLight) * 1.0 / 255;
					outputColor = objects[i]->mat.getColor(normal, directionalLight, ambientLight, pointLight, ptLightDir, -r.dir, reflectionColor, refractionColor);
					zbuffer = intersectionPoint.z;
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