#include "scenes.h"
std::vector<std::shared_ptr<primitive>> scene1()
{
	vec white(1.0, 1.0, 1.0), whiteSpec(1, 1, 1), red(1.0, 0, 0.0), redSpec(0.5, 1.0, 0.5), gray(0.75, 0.75, 0.75),
	green(0.0, 1.0, 0.0), greenSpec(0.5, 1, 0.5), yellow(1, 1, 0), blue(0.0, 0.0, 1.0), purple(1.0, 0.0, 1.0);

	material wm(0.8, 0.1, 0.3, white, whiteSpec, 4, 0), rm(0.8, 0.8, 0.1, red, redSpec, 32, 0),
	ym(0.9, 1.0, 0.1, yellow, whiteSpec, 4, 0), ym2(0.9, 0.5, 0.1, yellow, whiteSpec, 4, 0),
	gm(0.7, 0.5, 0.1, green, greenSpec, 64, 0), bm(0.9, 1.0, 0.1, blue, whiteSpec, 4, 0),
	bm2(0.9, 0.9, 0.1, blue, whiteSpec, 32, 0), pm(0.7, 0.2, 0.1, purple, whiteSpec, 16, 0), 
	refm(0.0, 0.1, 0.1, gray, whiteSpec, 10, 0.9);
	
	std::vector<sphere> spheres = {};
	std::vector<plane> planes = {};
	std::vector<material> mats = { rm, gm, bm, refm, ym2, pm, wm, refm};
	// mats = {refm, refm, refm, refm};

	std::vector<std::shared_ptr<primitive>> objects = {};

	for (int i = 0; i < 4; i++) {
		double x = double(i)/2.0-0.5;
		double y = 0;
		plane p(x-0.3, 0.0+y, -3,
				x, -0.2+y, 0,
				x+0.3, 0.0+y, -3,
				mats[i]);
		objects.push_back(std::make_shared<plane>(p));
		sphere s(x, y+0.1, -2-0.1*i, 0.25, mats[i+4]);
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

	material refractiveMaterial(0.0, 0.5, 0.1, gray, whiteSpec, 30, 0.0, 1.45);
	plane refractivePlane(
		0.5, 0.1, -0.5,
		0.15, -0.2, -0.0,
		0.5, -0.2, -0.0,
		refractiveMaterial);
	objects.push_back(std::make_shared<plane>(refractivePlane));
	sphere refractiveSphere(-0.25, 0.2, -1, 0.2, refractiveMaterial);
	objects.push_back(std::make_shared<sphere>(refractiveSphere));
	return objects;
}

std::vector<std::shared_ptr<primitive>> scene2()
{
	std::vector<std::shared_ptr<primitive>> objects = {};
	vec whiteSpec(1, 1, 1), gray(0.75, 0.75, 0.75), green(0.0, 1.0, 0.0), greenSpec(0.5, 1, 0.5);
	material refm(0.0, 0.1, 0.1, gray, whiteSpec, 10, 0.9), gm(0.7, 0.5, 0.1, green, greenSpec, 64, 0);
    sphere s1(-0.25, 0, 0, 0.15, gm);
    sphere s2(0.25, 0, 0, 0.15, gm);

	objects.push_back(std::make_shared<sphere>(s1));
	objects.push_back(std::make_shared<sphere>(s2));
	return objects;
}

float randF(float range) {
    int random_int = rand();
    return ((random_int % 200) - 100)/100.0 * range;
}
std::vector<std::shared_ptr<primitive>> sceneBoundingVolumeDemo()
{
    std::vector<std::shared_ptr<primitive>> objects = {};

	vec white(1.0, 1.0, 1.0), whiteSpec(1, 1, 1), red(1.0, 0, 0.0), redSpec(0.5, 1.0, 0.5), gray(0.75, 0.75, 0.75),
	green(0.0, 1.0, 0.0), greenSpec(0.5, 1, 0.5), yellow(1, 1, 0), blue(0.0, 0.0, 1.0), purple(1.0, 0.0, 1.0);

	material wm(0.8, 0.1, 0.3, white, whiteSpec, 4, 0), rm(0.8, 0.8, 0.1, red, redSpec, 32, 0),
	ym(0.9, 1.0, 0.1, yellow, whiteSpec, 4, 0),
	gm(0.7, 0.5, 0.1, green, greenSpec, 64, 0),
	bm(0.9, 0.9, 0.1, blue, whiteSpec, 32, 0), pm(0.7, 0.2, 0.1, purple, whiteSpec, 16, 0), 
	reflm(0.0, 0.1, 0.1, gray, whiteSpec, 10, 0.8), refrm(0.0, 0.5, 0.1, gray, whiteSpec, 30, 0.0, 1.5);
	
	std::vector<material> mats = {wm, rm, ym, gm, bm, pm, reflm, refrm};
	int totalCount = 0;
	float jitter = 0.2;
	for (double x = -1.5; x < 1.5; x += 0.25) {
		int ycount = 0;
		for (double y = -1.5; y < 1.5; y += 0.25) {
			for (double z = 0.5; z < 6; z+=0.25) {
				sphere s1(x+0.25*y+randF(jitter), y+randF(jitter), -z-0.25*(ycount%2)+randF(jitter), 0.06, mats[totalCount%mats.size()]);
				objects.push_back(std::make_shared<sphere>(s1));
				totalCount++;
			}
			ycount++;
		}		
	}
		
	return objects;
}
