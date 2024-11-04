#pragma once
#include <string>
#include "Shader.h"
#include "Model.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "glm/ext.hpp"
#include "PostProcessing.h"
class Skybox: private PostProcessing
{
	int width, height, nrComponents;
	float* data;
	
	Shader hdrToCubemapShader;
	Shader irradianceShader;
	Shader prefilterShader;

	unsigned int captureFBO, captureRBO;

	Shader brdfShader;
	Shader skyboxShader;
	unsigned int irradianceMapFBO, irradianceMapRBO;
	unsigned int prefilterMapFBO, prefilterMapRBO;
public:

	unsigned int envCubemap;
	static unsigned int brdfLUTTexture;
	static unsigned int irradianceMap;
	static unsigned int prefilterMap;

	//Ignore this
	void calculateCubemapfromHDRI();
	unsigned int TextureID;
	Skybox(const std::string& skyFile);
	Skybox() = default;
	void drawSkybox();
	
	//Calculates irradiance map from input hdr image
	void calculateIrradiance();

	void calculatePrefilteredMap();

};

