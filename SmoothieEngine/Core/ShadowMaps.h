#pragma once
#include "Shader.h"
#include "glm/ext.hpp"
#include "PostProcessing.h"
class ShadowMaps: private PostProcessing
{
	int imageWidth, imageHight, screenWidth, screenHeight;

	//Lighting stuff
	float near_plane = -7.5f, far_plane = 15.0f;
	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;

public:
	ShadowMaps(int imageWidth = 2048, int imageHight = 2048, int screenWidth = 1280, int screenHeight = 720);
	
	Shader shader;
	void firstPass();
	void secondPass() const;
	
	unsigned int depthMapTexture;
	unsigned int depthMapFB;

	float* lightSpaceMatrixPtr();
};

