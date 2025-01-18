#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "ray.h"
#include "sphere.h"
#include "material.h"
#include "plane.h"

vec bgColor(0.4, 0.4, 0.4);
vec cameraPosition(0, 0, 1);
vec cameraFace(0, 0, 0);
vec cameraUp(0, 1, 0);
int fov = 90;

vec dirToLight(0.0, 1.0, 0.0);
vec lightColor(1, 1, 1);
directionalLight mainlight(lightColor, 1, dirToLight);
vec ambientLightColor(0.1, 0.1, 0.1);
light ambientLight(ambientLightColor, 1);

int pixels_width = 400;
//int pixels_height = static_cast<int>(pixels_width * 9 / 16);
int pixels_height = pixels_width;
double units_width = 6;
//double units_height = units_width * 9.0 / 16.0;
double units_height = units_width;

vec colorAtRay(ray r, std::vector<sphere> spheres, std::vector<plane> planes, int bounceCount) {
	vec refColor = bgColor;
	vec outputColor = colorize(bgColor);
	double zbuffer = -999999999;
		
	// Reflections or diffuse for all planes
	for (int i = 0; i < planes.size(); i++) {
		std::vector<vec> intersectionInfo = planes[i].getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			vec normal = intersectionInfo[1];
			ray toLight(intersectionPoint, mainlight.direction);
			double eta = 0.001;
			vec reflectionOrigin = intersectionPoint + normal * eta;
			vec reflectionDirection = r.dir - normal * 2 * r.dir.dot(normal);
			ray reflectionRay(reflectionOrigin, reflectionDirection);
			// Calculate color
			vec refColor = bgColor;
			if (intersectionPoint.z > zbuffer) {
				outputColor = planes[i].mat.getColor(normal, mainlight, ambientLight, -r.dir, refColor);
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
							vec refColor = colorAtRay(reflectionRay, spheres, planes, bounceCount + 1) * 1.0 / 255;
							outputColor = planes[i].mat.getColor(normal, mainlight, ambientLight, -r.dir, refColor);
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
							vec refColor = colorAtRay(reflectionRay, spheres, planes, bounceCount + 1) * 1.0 / 255;
							outputColor = planes[i].mat.getColor(normal, mainlight, ambientLight, -r.dir, refColor);
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
			ray toLight(intersectionPoint, mainlight.direction);
			double eta = 0.0001;
			vec reflectionOrigin = intersectionPoint + normal * eta;
			vec reflectionDirection = r.dir - normal * 2 * r.dir.dot(normal);
			ray reflectionRay(reflectionOrigin, reflectionDirection);
			// Calculate color
			if (intersectionPoint.z > zbuffer) {
				outputColor = spheres[i].mat.getColor(normal, mainlight, ambientLight, -r.dir, refColor);
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
						vec refColor = colorAtRay(reflectionRay, spheres, planes, bounceCount + 1) * 1.0 / 255;
						outputColor = spheres[i].mat.getColor(normal, mainlight, ambientLight, -r.dir, refColor);
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
							vec refColor = colorAtRay(reflectionRay, spheres, planes, bounceCount + 1) * 1.0 / 255;
							outputColor = spheres[i].mat.getColor(normal, mainlight, ambientLight, -r.dir, refColor);
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
			ray toLight(intersectionPoint, mainlight.direction);
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
			ray toLight(intersectionPoint, mainlight.direction);
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

float randFloat(float range) {
	if (rand() % 2 == 0)
		return (rand() % 100) / (range * 100);
	return -(rand() % 100) / (range * 100);
}

int main() {
	// Setup file
	std::ofstream output;
	output.open("output.ppm");
	output << "P3\n";
	output << pixels_width << ' ' <<  pixels_height << '\n' << 255 << '\n';
	
	// Rendering
	vec white(1.0, 1.0, 1.0);
	vec whiteSpec(1, 1, 1);
	vec redSpec(0.5, 1.0, 0.5);
	vec gray(0.75, 0.75, 0.75);
	vec greenSpec(0.5, 1, 0.5);
	vec red(1.0, 0.0, 0.0);
	vec green(0.0, 1.0, 0.0);
	vec yellow(1, 1, 0);
	vec blue(0.0, 0.0, 1.0);
	vec purple(1.0, 0.0, 1.0);

	material wm(0.8, 0.1, 0.3, white, whiteSpec, 4, 0);
	material rm(0.8, 0.8, 0.1, red, redSpec, 32, 0);
	material ym(0.9, 1.0, 0.1, yellow, whiteSpec, 4, 0);
	material ym2(0.9, 0.5, 0.1, yellow, whiteSpec, 4, 0);

	material gm(0.7, 0.5, 0.1, green, greenSpec, 64, 0);
	material bm(0.9, 1.0, 0.1, blue, whiteSpec, 4, 0);
	material bm2(0.9, 0.9, 0.1, blue, whiteSpec, 32, 0);

	material pm(0.7, 0.2, 0.1, purple, whiteSpec, 16, 0);
	material refm(0.0, 0.1, 0.1, gray, whiteSpec, 10, 0.9);

	sphere whiteSphere(0.5, 0, -0.15, 0.05, wm);
	sphere redSphere(0.3, 0.0, -0.1, 0.08, rm);
	sphere greenSphere(-0.6, 0, 0, 0.3, gm);
	sphere blueSphere(0, -10000.5, 0, 10000, bm);
	sphere purpleSphere(0, 0, 0, 0.4, pm);
	sphere reflectiveSphere(0, 0.3, -1, 0.25, refm);
	sphere reflectiveSphere2(0.1, -0.55, 0.25, 0.3, refm);

	plane blueTriangle(0.0, -0.7, -0.5,
		1.0, 0.4, -1.0,
		0.0, -0.7, -1.5,
		bm);
	plane blueTriangle2(0.3, -0.3, -0.4,
		0.0, 0.3, -0.1,
		-0.3, -0.3, 0.2,
		bm2);
	plane yellowTriangle(0.0, -0.7, -0.5,
		0.0, -0.7, -1.5,
		-1.0, 0.4, -1.0, ym);
	plane yellowTriangle2(-0.2, 0.1, 0.1,
		-0.2, -0.5, 0.2,
		-0.2, 0.1, -0.3, ym2);

	sphere s1(0.5, 0, -0.15, 0.05, wm);
	sphere s2(0.3, 0.0, -0.1, 0.08, rm);
	sphere s3(-0.6, 0, 0, 0.3, gm);
	sphere s4(0, -10000.5, 0, 10000, bm);
	sphere s5(0, 0, 0, 0.4, pm);
	sphere s6(0, 0.3, -1, 0.25, refm);
	plane t1(-1.0, 0.8, -0.5,
		0.25, -0.4, -2.0,
		-0.5, 0.5, -1.5,
		bm);
	plane t2(0.3, -0.3, -0.4,
		0.0, 0.3, -0.1,
		-0.3, -0.3, 0.2,
		bm2);
	plane t3(0.0, -0.7, -0.5,
		0.0, -0.7, -1.5,
		-1.0, 0.4, -1.0, ym);
	plane t4(-0.2, 0.1, 0.1,
		-0.2, -0.5, 0.2,
		-0.2, 0.1, -0.3, ym2);
	std::vector<sphere> spheres = {};
	std::vector<plane> planes = {};
	std::vector<material> mats = { rm, gm, bm, pm, wm, refm, ym };
	int count = 50;
	for (int i = 0; i < count; i++) {
		float x = sin(i * 72);
		float y = sin(i * 36);
		float z = -1.0/5.0*i;
		plane p(-0.5+x, -0.2+y, 0.0+z,
			-0.2+x, -0.2+y, 0.0+z,
			-0.5+x, 0.2+y, -5+z, mats[i%mats.size()]);
		planes.push_back(p);

		sphere s(randFloat(2), randFloat(2), randFloat(2), randFloat(10), mats[i % mats.size()]);
		spheres.push_back(s);
	}
	for (int i = pixels_height; i >=0 ; i--) {
		for (int j = 0; j < pixels_width; j++) {
			double x_percent = double(j) / double(pixels_width - 1);
			double y_percent = double(i) / double(pixels_height - 1);
			vec pixel(x_percent * units_width - units_width/2, y_percent * units_height - units_height/2, -4.4);
			vec dir = pixel - cameraPosition;
			dir = normalize(dir);
			vec outColor = colorAtRay(ray(cameraPosition, dir), spheres, planes, 0);
			output << outColor;
		}
		std::cout << "Row: " << i << "\n";
	}
	output.close();
	return 0;
}