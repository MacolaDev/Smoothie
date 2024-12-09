#pragma once
#include "SmoothieCore.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "PostProcessing.h"

class DeferredShading: private PostProcessing
{
	static unsigned int gBufferRenderbuffer;
	static unsigned int lightPassRenderbuffer;
public:
	static unsigned int gBuffer;
	static unsigned int gPosition, gNormal, gAlbedoSpec, gMRAO;
	
	static unsigned int lightPassFramebuffer;
	static unsigned int lightPassFramebufferTexture;

	static Shader gBufferShader;
	static Shader lightingPassShader;


	static void generateBuffersTextures();
	
	//Initilizes deffered shading
	static void initilize();

	static void bindGBuffer();

	//Renders stuff from gBuffer and binds light pass 
	static void renderPBR();

	//Copies stuff from light pass into default framebuffer and renderbuffer.
	//Must be called at the end of render pipeline.
	static void finilize();
};

