#pragma once
#include "SmoothieCore.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "PostProcessing.h"

class DeferredShading: private PostProcessing
{
	static unsigned int TempRenderBuffer;
public:
	static unsigned int gBuffer;
	static unsigned int gPosition, gNormal, gAlbedoSpec, gMRAO;

	static Shader gBufferShader;
	static Shader lightingPassShader;


	static void generateBuffersTextures();
	
	//Initilizes deffered shading
	static void initilize();

	static void bindGBuffer();
	static void renderPBR();
};

