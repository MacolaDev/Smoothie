#pragma once
#include <vector>
#include "Shader.h"
#include "PostProcessing.h"
#include "SmoothieMath.h"
#include "SmoothieCore.h"
#include "DeferredShading.h"
#include "Texture.h"
using namespace SmoothieMath;
using namespace Smoothie;

//Screen Space Ambient Occlusion.
//TODO: Add disable/enable of SSAO in SmoothieCore.

class DeferredShading;
class SSAO: private PostProcessing
{
	static unsigned int ssaoFBO;
	static Texture noiseTexture, ssaoColorBuffer;

	static Shader shaderSSAO;
	static Shader blure;
	friend class DeferredShading;

	static unsigned int BluredFBO;
	static Texture BluredTexture;

	static std::vector<Vector3> ssaoNoise;
	static std::vector<Vector3> ssaoKernel;
	static void generateNoise();


public:

	SSAO() = default;
	static void generateSSAOTextures(int SCR_WIDTH, int SCR_HEIGHT);
	static void onRender();

};

