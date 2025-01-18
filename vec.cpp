#include "vec.h"

vec::vec(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec::vec(double u, double v)
{
    this->x = u;
    this->y = v;
    this->z = 0;
}

vec vec::operator+(vec v)
{
    return vec(this->x+v.x, this->y+v.y, this->z+v.z);
}
vec vec::operator-()
{
    return vec(this->x* -1, this->y* -1, this->z * -1);
}
vec vec::operator-(vec v)
{
    return vec(this->x - v.x, this->y - v.y, this->z - v.z);
}
vec vec::operator*(double s)
{
    return vec(this->x * s, this->y * s, this->z * s);
}
vec vec::cross(vec v) {
    return vec(this->y * v.z - this->z * v.y,
        this->z * v.x - this->x * v.z,
        this->x * v.y - this->y * v.x);
}
vec vec::operator/(double s)
{
    return vec(this->x / s, this->y / s, this->z / s);
}

double vec::length()
{
    return std::sqrt(x*x + y*y + z*z);
}

vec vec::operator*(vec v)
{
    return vec(this->x * v.x, this->y * v.y, this->z * v.z);
}

double vec::dot(vec v)
{
    return v.x * this->x + v.y * this->y + v.z * this->z;
}

vec vec::operator/(vec v)
{
    return vec(this->x / v.x, this->y / v.y, this->z / v.z);
}

std::ostream& operator<<(std::ostream& out, vec& vector)
{
    return out << vector.x << " " << vector.y << " " << vector.z << " ";
}
