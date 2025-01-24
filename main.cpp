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

// scene info
vec bgColor(0.4, 0.4, 0.4);
vec cameraPosition(0, 0, 1);
vec cameraFace(0, 0, 0);
vec cameraUp(0, 1, 0);
int fov = 90;

vec dirToLight(0.0, 1.0, 0.0);
vec lightColor(1, 1, 1);
directionalLight mainlight(lightColor, 0.5, dirToLight);
pointLight ptLight(lightColor, 1, {0.1, 0.0, 0.2});
vec ambientLightColor(0.1, 0.1, 0.1);
light ambientLight(ambientLightColor, 1);

int pixels_width = 400;
//int pixels_height = static_cast<int>(pixels_width * 9 / 16);
int pixels_height = pixels_width;
double units_width = 6;
//double units_height = units_width * 9.0 / 16.0;
double units_height = units_width;

int main() {
	// Setup file
	std::ofstream output;
	output.open("output.ppm");
	output << "P3\n";
	output << pixels_width << ' ' <<  pixels_height << '\n' << 255 << '\n';
	
	// Rendering
	std::vector<std::shared_ptr<primitive>> objects = scene1();
	
	int samples = 6;
	auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = pixels_height; i >=0 ; i--) {
		for (int j = 0; j < pixels_width; j++) {
			double x_percent = double(j) / double(pixels_width - 1);
			double y_percent = double(i) / double(pixels_height - 1);
			vec pixel(x_percent * units_width - units_width/2, y_percent * units_height - units_height/2, -4.4);
			vec out_color(0, 0, 0);
			for (int a = 0; a < samples; a++) {
				for (int b = 0; b < samples; b++) {					
					vec random_jitter(randFloat(0.01), randFloat(0.01));
					vec dir = pixel - cameraPosition + random_jitter;
					dir = normalize(dir);
					out_color += colorAtRay(ray(cameraPosition, dir), objects, 0, bgColor, mainlight, ambientLight, ptLight);
				}
			}
			out_color /= samples*samples;
			output << out_color;
		}
		std::cout << "Row: " << i << "\n";
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
	std::cout << "Ray casting loop took " << duration.count() << " seconds.";

	output.close();
	return 0;
}