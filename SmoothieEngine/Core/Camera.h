#pragma once
#include "SmoothieMath.h"


using namespace SmoothieMath;
class SmoothieCore;
namespace Smoothie {

	class Camera
	{
		Matrix4x4 cameraMatrix;
		Matrix4x4 projectionMatrix;
		float zNear, zFar, fovy, aspec;
		friend class SmoothieCore;
	public:

		Vector3 cameraPos, cameraFront, cameraUp;

		void updateCameraPosition(const Vector3& position);
		void updateCameraFront(const Vector3& front);

		void updateCameraMatrix();
		void updateProjectionMatrix(float fovy, float aspec, float zNear, float zFar);
		float* matrixPtr();

		Camera(const Vector3& cameraPos, const Vector3& cameraFront, const Vector3& cameraUp, float fovy, float aspec, float zNear, float zFar);
		Camera();
	};

}