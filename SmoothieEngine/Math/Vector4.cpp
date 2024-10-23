#include "Vector4.h"
#include "iostream"

using namespace SmoothieMath;
Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float Vector4::length() const
{
	float x2 = x*x;
	float y2 = y*y;
	float z2 = z*z;
	float w2 = w*w;
	
	return std::sqrt(x2 + y2 + z2 + w2);
}

void Vector4::normalizeVector()
{

	float vectorLength = length();

	if (vectorLength == 0) { 
		std::cout << "Vector has 0 lenght. Cant normalize it." << std::endl;
		return;
	}

	x = x / vectorLength;
	y = y / vectorLength;
	z = z / vectorLength;
	w = w / vectorLength;
}


float* Vector4::dataPointer()
{
	return &x;
}

Vector4 Vector4::operator + (const Vector4& other) const
{
	return Vector4(
		x + other.x, 
		y + other.y,
		z + other.z,
		w + other.w);
}

Vector4 Vector4::operator * (const Vector4& other) const
{
	return Vector4(
		x * other.x,
		y * other.y,
		z * other.z,
		w * other.w);
}

Vector4 Vector4::operator * (const float& scalar) const
{
	return Vector4(
		x * scalar,
		y * scalar,
		z * scalar,
		w * scalar);
}

void Vector4::print() const
{
	std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl;
}