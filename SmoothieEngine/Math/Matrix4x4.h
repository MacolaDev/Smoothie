#pragma once
#include "Vector4.h"
#include "Vector3.h"

namespace SmoothieMath {
class Matrix3x3;
	class Matrix4x4
	{
		float matrix[4][4];
	
	public:
		friend class Matrix3x3;
		Matrix4x4();
		Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);
		Matrix4x4(float data[4][4]);
		void scaleMatrix( float x,  float y,  float z);
		void translateMatrix( float x,  float y,float z);

		void orthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
		
		//Degrees
		void perspectiveProjection(float fovy, float aspec, float zNear,  float zFar);

		void xAxisRotationDegrees( float degrees);
		void xAxisRotationRadians( float radians);

		void yAxisRotationDegrees( float degrees);
		void yAxisRotationRadians( float radians);

		void zAxisRotationDegrees( float degrees);
		void zAxisRotationRadians( float radians);

		void lookAtMatrix(const Vector3& eye, const Vector3& center,  Vector3 up);

		float* dataPointer();
	
		void inverse();

		void transpose();
		
		//TODO: Find better solution 
		Matrix4x4 operator * (const Matrix4x4& other);
		Vector4 operator [] (int index) const;
	};
	
}