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

// scene info
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

int main() {
	// Setup file
	std::ofstream output;
	output.open("output.ppm");
	output << "P3\n";
	output << pixels_width << ' ' <<  pixels_height << '\n' << 255 << '\n';
	
	// Rendering
	vec white(1.0, 1.0, 1.0), whiteSpec(1, 1, 1), red(1.0, 0, 0.0), redSpec(0.5, 1.0, 0.5), gray(0.75, 0.75, 0.75),
	green(0.0, 1.0, 0.0), greenSpec(0.5, 1, 0.5), yellow(1, 1, 0), blue(0.0, 0.0, 1.0), purple(1.0, 0.0, 1.0);

	material wm(0.8, 0.1, 0.3, white, whiteSpec, 4, 0), rm(0.8, 0.8, 0.1, red, redSpec, 32, 0),
	ym(0.9, 1.0, 0.1, yellow, whiteSpec, 4, 0), ym2(0.9, 0.5, 0.1, yellow, whiteSpec, 4, 0),
	gm(0.7, 0.5, 0.1, green, greenSpec, 64, 0), bm(0.9, 1.0, 0.1, blue, whiteSpec, 4, 0),
	bm2(0.9, 0.9, 0.1, blue, whiteSpec, 32, 0), pm(0.7, 0.2, 0.1, purple, whiteSpec, 16, 0), 
	refm(0.0, 0.1, 0.1, gray, whiteSpec, 10, 0.9);
	
	std::vector<sphere> spheres = {};
	std::vector<plane> planes = {};
	std::vector<material> mats = { rm, gm, bm, refm};

	std::vector<std::shared_ptr<primitive>> objects = {};

	for (int i = 0; i < 4; i++) {
		double x = double(i)/2.0-0.5;
		double y = 0;
		plane p(x-0.3, -0.2+y, -3,
				x, -0.2+y, 0,
				x+0.3, -0.2+y, -3,
				mats[i%4]);
		objects.push_back(std::make_shared<plane>(p));
		sphere s(x, y+0.1, -2-0.1*i, 0.25, mats[(i+1)%4]);
		objects.push_back(std::make_shared<sphere>(s));
	}
	plane p2(-1, 0.5, -0.5,
			-0.5, -0.5, -0.5,
			-1, 0.25, -4,
			refm);
	objects.push_back(std::make_shared<plane>(p2));
	plane white_plane(
		1, 0.1, -0.5,
		1, 1, -4,
		1.5, 0.1, -0.5,
		pm);
	objects.push_back(std::make_shared<plane>(white_plane));

	int samples = 1;
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
					out_color += colorAtRay(ray(cameraPosition, dir), objects, 0, bgColor, mainlight, ambientLight);
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