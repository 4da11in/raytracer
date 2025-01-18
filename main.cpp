#include <iostream>
#include <fstream>
#include <vector>
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
	vec white(1.0, 1.0, 1.0), whiteSpec(1, 1, 1), red(1.0, 0, 0.0), redSpec(0.5, 1.0, 0.5), gray(0.75, 0.75, 0.75),
	green(0.0, 1.0, 0.0), greenSpec(0.5, 1, 0.5), yellow(1, 1, 0), blue(0.0, 0.0, 1.0), purple(1.0, 0.0, 1.0);

	material wm(0.8, 0.1, 0.3, white, whiteSpec, 4, 0), rm(0.8, 0.8, 0.1, red, redSpec, 32, 0),
	ym(0.9, 1.0, 0.1, yellow, whiteSpec, 4, 0), ym2(0.9, 0.5, 0.1, yellow, whiteSpec, 4, 0),
	gm(0.7, 0.5, 0.1, green, greenSpec, 64, 0), bm(0.9, 1.0, 0.1, blue, whiteSpec, 4, 0),
	bm2(0.9, 0.9, 0.1, blue, whiteSpec, 32, 0), pm(0.7, 0.2, 0.1, purple, whiteSpec, 16, 0), 
	refm(0.0, 0.1, 0.1, gray, whiteSpec, 10, 0.9);

	sphere whiteSphere(0.5, 0, -0.15, 0.05, wm);
	sphere redSphere(0.3, 0.0, -0.1, 0.08, rm);
	sphere greenSphere(-0.6, 0, 0, 0.3, gm);
	sphere blueSphere(0, -10000.5, 0, 10000, bm);
	sphere purpleSphere(0, 0, 0, 0.4, pm);
	sphere reflectiveSphere(0, 0.3, -1, 0.25, refm);
	sphere reflectiveSphere2(0.1, -0.55, 0.25, 0.3, refm);

	// plane blueTriangle(0.0, -0.7, -0.5,
	// 	1.0, 0.4, -1.0,
	// 	0.0, -0.7, -1.5,
	// 	bm);
	// plane blueTriangle2(0.3, -0.3, -0.4,
	// 	0.0, 0.3, -0.1,
	// 	-0.3, -0.3, 0.2,
	// 	bm2);
	// plane yellowTriangle(0.0, -0.7, -0.5,
	// 	0.0, -0.7, -1.5,
	// 	-1.0, 0.4, -1.0, ym);
	// plane yellowTriangle2(-0.2, 0.1, 0.1,
	// 	-0.2, -0.5, 0.2,
	// 	-0.2, 0.1, -0.3, ym2);

	// sphere s1(0.5, 0, -0.15, 0.05, wm);
	// sphere s2(0.3, 0.0, -0.1, 0.08, rm);
	// sphere s3(-0.6, 0, 0, 0.3, gm);
	// sphere s4(0, -10000.5, 0, 10000, bm);
	// sphere s5(0, 0, 0, 0.4, pm);
	// sphere s6(0, 0.3, -1, 0.25, refm);
	// plane t1(-1.0, 0.8, -0.5,
	// 	0.25, -0.4, -2.0,
	// 	-0.5, 0.5, -1.5,
	// 	bm);
	// plane t2(0.3, -0.3, -0.4,
	// 	0.0, 0.3, -0.1,
	// 	-0.3, -0.3, 0.2,
	// 	bm2);
	// plane t3(0.0, -0.7, -0.5,
	// 	0.0, -0.7, -1.5,
	// 	-1.0, 0.4, -1.0, ym);
	// plane t4(-0.2, 0.1, 0.1,
	// 	-0.2, -0.5, 0.2,
	// 	-0.2, 0.1, -0.3, ym2);
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

	auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = pixels_height; i >=0 ; i--) {
		for (int j = 0; j < pixels_width; j++) {
			double x_percent = double(j) / double(pixels_width - 1);
			double y_percent = double(i) / double(pixels_height - 1);
			vec pixel(x_percent * units_width - units_width/2, y_percent * units_height - units_height/2, -4.4);
			vec dir = pixel - cameraPosition;
			dir = normalize(dir);
			vec outColor = colorAtRay(ray(cameraPosition, dir), spheres, planes, 0, bgColor, mainlight, ambientLight);
			output << outColor;
		}
		std::cout << "Row: " << i << "\n";
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
	std::cout << "Ray casting loop took " << duration.count() << " seconds.";

	output.close();
	return 0;
}