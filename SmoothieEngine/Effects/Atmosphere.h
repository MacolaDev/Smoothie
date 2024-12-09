#pragma once
#include "SmoothieMath.h"
#include "Shader.h"

struct Sun 
{
	SmoothieMath::Vector3 color = { 1.0f, 1.0f, 1.0f };
	
	//Hight of a sun on a sky, from -90 to 90 degrees
	float zenithAngle = 20.0f;
	
	//Rotation of a sun, from 0 to 360 degrees
	float azimuthAngle = 45.0f;
	
};

class Atmosphere
{
	void drawSun() const;

	Atmosphere() = default;

private:
	static Shader drawSun;

};

