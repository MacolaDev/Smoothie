#pragma once
#include <vector>
#include <glm/ext.hpp>
#include "Shader.h"
#include "VertexBuffer.h"
#include "PostProcessing.h"
#include "SmoothieMath.h"
#include "SmoothieCore.h"
#include "DeferredShading.h"

using namespace SmoothieMath;
//Screen Space Ambient Occlusion.
//This effect can be enabled/disabled in SmoothieCore.
//NOT YET IMPLEMENTED PROPERLY! DOES NOT WORK YET!
class DeferredShading;
class SSAO: private PostProcessing
{
	static unsigned int ssaoFBO;
	static unsigned int noiseTexture;
	static unsigned int ssaoColorBuffer;
	
	static Shader shaderSSAO;
	static Shader blure;
	friend class DeferredShading;

	static unsigned int BluredFBO;
	static unsigned int BluredTexture;
	
	static std::vector<Vector3> ssaoNoise;
	static std::vector<Vector3> ssaoKernel;
	static void generateNoise();
public:

	SSAO() = default;
	static void generateSSAOTextures(int SCR_WIDTH, int SCR_HEIGHT);
	static void onRender();

};

