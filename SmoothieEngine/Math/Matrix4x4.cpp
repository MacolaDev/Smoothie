#include "Matrix4x4.h"
#include "SmoothieMath.h"
#include <iostream>
using namespace SmoothieMath;

Matrix4x4::Matrix4x4()
{
	matrix[0][0] = 1, matrix[0][1] = 0, matrix[0][2] = 0, matrix[0][3] = 0,
	matrix[1][0] = 0, matrix[1][1] = 1, matrix[1][2] = 0, matrix[1][3] = 0,
	matrix[2][0] = 0, matrix[2][1] = 0, matrix[2][2] = 1, matrix[2][3] = 0,
	matrix[3][0] = 0, matrix[3][1] = 0, matrix[3][2] = 0, matrix[3][3] = 1;
}

Matrix4x4::Matrix4x4( const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
{
	matrix[0][0] = row0.x, matrix[1][0] = row0.y, matrix[2][0] = row0.z, matrix[3][0] = row0.w;
	matrix[0][1] = row1.x, matrix[1][1] = row1.y, matrix[2][1] = row1.z, matrix[3][1] = row1.w;
	matrix[0][2] = row2.x, matrix[1][2] = row2.y, matrix[2][2] = row2.z, matrix[3][2] = row2.w;
	matrix[0][3] = row3.x, matrix[1][3] = row3.y, matrix[2][3] = row3.z, matrix[3][3] = row3.w;
}																							   

float* Matrix4x4::dataPointer()
{
	return &matrix[0][0];
}

void Matrix4x4::scaleMatrix(float x, float y, float z)
{
	matrix[0][0] = x;
	matrix[1][1] = y;
	matrix[2][2] = z;
}

void Matrix4x4::translateMatrix( float x,  float y, float z)
{
	matrix[3][0] = x;
	matrix[3][1] = y;
	matrix[3][2] = z;
}

void Matrix4x4::orthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float r_l = right - left;
	if (r_l == 0) return;

	float t_b = top - bottom;
	if (t_b == 0) return;

	float f_n = zFar - zNear;
	if (f_n == 0) return;

	matrix[0][0] = 2 / r_l;
	matrix[1][1] = 2 / t_b;
	matrix[2][2] = -2 / f_n;

	matrix[0][3] = -(right + left) / r_l;
	matrix[1][3] = -(top + bottom) / t_b;
	matrix[2][3] = -(zFar + zNear) / f_n;

}

void Matrix4x4::perspectiveProjection( float fovy, float aspec, float zNear, float zFar)
{	
	float toRadians = fovy*pi/180;
	float f = 1/std::tan(toRadians/2);
	matrix[0][0] = f / aspec;
	matrix[1][1] = f;
	float value = zNear - zFar;
	if (value == 0) return;
	matrix[2][2] = (zFar + zNear) / value;
	matrix[3][2] = (2 * zFar * zNear) / value;
	matrix[2][3] = -1;
	matrix[3][3] = 0;
}

void Matrix4x4::xAxisRotationDegrees( float degrees)
{
	float value = degrees * pi / 180;
	matrix[1][1] = std::cos(value); matrix[2][1] = -std::sin(value);
	matrix[1][2] = std::sin(value); matrix[2][2] =  std::cos(value);
}

void Matrix4x4::xAxisRotationRadians( float radians)
{
	matrix[1][1] = std::cos(radians); matrix[2][1] = -std::sin(radians);
	matrix[1][2] = std::sin(radians); matrix[2][2] =  std::cos(radians);
}

void Matrix4x4::yAxisRotationDegrees( float degrees)
{
	float value = degrees * pi / 180;
	matrix[0][0] = std::cos(value); matrix[0][2] = std::sin(value);
	matrix[2][0] = -std::sin(value); matrix[2][2] = std::cos(value);
}

void Matrix4x4::yAxisRotationRadians( float radians)
{
	matrix[0][0] = std::cos(radians); matrix[0][2] = std::sin(radians);
	matrix[2][0] = -std::sin(radians); matrix[2][2] = std::cos(radians);
}

void Matrix4x4::zAxisRotationDegrees( float degrees)
{
	float value = degrees * pi / 180;
	matrix[0][0] = std::cos(value); matrix[0][1] = -std::sin(value);
	matrix[1][0] = std::sin(value); matrix[1][1] = std::cos(value);
}

void Matrix4x4::zAxisRotationRadians( float radians)
{
	matrix[0][0] = std::cos(radians); matrix[0][1] = -std::sin(radians);
	matrix[1][0] = std::sin(radians); matrix[1][1] = std::cos(radians);
}

void Matrix4x4::lookAtMatrix(const Vector3& eye, const Vector3& center, Vector3 up)
{
	Vector3 f = Vector3(center - eye);
	f.normalizeVector();
	Vector3 s = cross(f, up);
	s.normalizeVector();
	Vector3 u = cross( s, f);

	matrix[0][0] = s.x;
	matrix[1][0] = s.y;
	matrix[2][0] = s.z;

	matrix[0][1] = u.x;
	matrix[1][1] = u.y;
	matrix[2][1] = u.z;

	matrix[0][2] = -f.x;
	matrix[1][2] = -f.y;
	matrix[2][2] = -f.z;

	matrix[3][0] = -dot(s, eye);
	matrix[3][1] = -dot(u, eye);
	matrix[3][2] = dot(f, eye);

}
