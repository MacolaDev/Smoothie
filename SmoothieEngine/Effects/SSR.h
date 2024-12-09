#pragma once
#include "PostProcessing.h"
#include "SmoothieCore.h"
#include "DeferredShading.h"
#include "Texture.h"
#include "Shader.h"
using namespace Smoothie;


class SSR: private PostProcessing
{
	static unsigned int fbo;
	static unsigned int fboblured;

	//Screen space reflection shader
	static Shader ssrshader;

	static Shader bluredshader;

	//Target where screen space reflection shader will save its renered texture

	static InternalTexture2D target;
public:
	static InternalTexture2D bluredTarget;

	SSR() = default;
	static void generateSSRTextures(int SCR_WIDTH, int SCR_HEIGHT);
	static void onRender();
};

