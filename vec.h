#pragma once
#include <iostream>
#include <fstream>

class vec
{
    public:
	    double x;
	    double y;
	    double z;
		vec(double x, double y, double z);
		vec(double u, double v);
        vec operator+(vec v);
		vec operator-(vec v);
		vec operator-();
		vec operator*(vec v);
		double dot(vec v);
		vec cross(vec v);
		vec operator/(vec v);
		vec operator*(double s);
		vec operator/(double s);


		double length();

		friend std::ostream& operator<<(std::ostream& os, vec& vector);
};

