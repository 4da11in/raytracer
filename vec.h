#pragma once
#include <iostream>
#include <fstream>
#include <cmath>

class vec
{
    public:
	    double x;
	    double y;
	    double z;
		vec(double x, double y, double z);
		vec(double u, double v);

        vec operator+(vec v);
        vec& operator+=(vec v);
		vec operator-();
		vec operator-(vec v);
		vec operator*(double s);
		vec operator*(vec v);
		vec operator/(double s);
        vec& operator/=(double s);
		double& operator[](int i);

		double length();

		friend std::ostream& operator<<(std::ostream& os, vec& vector);
};

double dot(vec v1, vec v2);
vec cross(vec v1, vec v2);
vec normalize(vec v);