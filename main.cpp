#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <chrono>

#include "ray.h"
#include "sphere.h"
#include "material.h"
#include "plane.h"
#include "colorAtRay.h"
#include "scenes.h"
#include "areaLight.h"
#include "boundingVolumeHierarchy.h"

// scene info
vec bgColor(0.4, 0.4, 0.4);
vec cameraPosition(0, 0, 1);
vec cameraFace(0, 0, 0);
vec cameraUp(0, 1, 0);
int fov = 90;

vec dirToLight(0.0, 1.0, 0.0);
vec lightColor(1, 1, 1);
directionalLight dirLight(lightColor, 0.5, dirToLight);
pointLight ptLight(lightColor, 1, {0.1, 0, 0.2});
areaLight arLight(0, 1, -2, {1,0,0}, {0,0,1}, 4, 4, {1,1,1}, 1);
vec ambientLightColor(1, 1, 1);
light ambientLight(ambientLightColor, 1);
std::vector<std::shared_ptr<nonAmbientLight>> lights;

int pixels_width = 400;
int pixels_height = pixels_width;
double units_width = 6;
double units_height = units_width;

int main() {
	// Setup file
	std::ofstream output;
	output.open("output.ppm");
	output << "P3\n";
	output << pixels_width << ' ' <<  pixels_height << '\n' << 255 << '\n';
	
	// initialize non-ambient lights list
	// lights.push_back(std::make_shared<pointLight>(ptLight));
	// lights.push_back(std::make_shared<directionalLight>(dirLight));
	lights.push_back(std::make_shared<areaLight>(arLight));
	
	std::vector<std::shared_ptr<primitive>> objects = sceneBoundingVolumeDemo();
	boundingVolumeHierarchy bvh({0,0,0},{0,0,0});
	// create bounding volume hierarchy
	int id = 0;
	for (std::shared_ptr<primitive> object : objects) {
		object->id = id++;
		boundingBox bbox = object->getBoundingBox();
		// bvh.boundingChildren.push_back(std::make_shared<boundingVolumeHierarchy>(bbox));
		bvh.primChildren.push_back(object);
		for (int dim = 0; dim < 3; dim++) {
			if (bbox.e1[dim] < bvh.e1[dim]) {
				bvh.e1[dim] = bbox.e1[dim];
			}
			if (bbox.e2[dim] > bvh.e2[dim]) {
				bvh.e2[dim] = bbox.e2[dim];
			}
		}
	}
	bvh.medianSplit(20, 2); // 20 5
	std::cout << "\ntotal count: " << objects.size() << '\n';
	std::cout << "e1/e2:" << bvh.e1 << " " << bvh.e2 << '\n';
	// std::cout << bvh << '\n';

	// Rendering loop
	int samples = 6;
	auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = pixels_height; i >=0 ; i--) {
		for (int j = 0; j < pixels_width; j++) {
			double x_percent = double(j) / double(pixels_width - 1);
			double y_percent = double(i) / double(pixels_height - 1);
			vec pixel(x_percent * units_width - units_width/2, y_percent * units_height - units_height/2, -4.4);
			vec out_color(0, 0, 0);
			vec dir = pixel - cameraPosition;
			
			for (int a = 0; a < samples; a++) {
				for (int b = 0; b < samples; b++) {
					ray r(cameraPosition, dir);
					vec randomJitter(randFloat(0.01), randFloat(0.01));
					r.dir += randomJitter;
					r.dir = normalize(r.dir);
					std::vector<vec> colorInfo = colorAtRay(r, bvh, bvh, 0, bgColor, ambientLight, lights, -999999999);
					out_color += colorInfo[0];
				}
			}
			out_color /= samples*samples;
			output << out_color;
		}
		if (i % 50 == 0) {
			std::cout << "\n" << i;
		} else {
			std::cout << ".";
		}
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
	std::cout << "\nRay casting loop took " << duration.count() << " seconds.";

	output.close();
	return 0;
}