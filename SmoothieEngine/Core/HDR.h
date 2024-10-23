#pragma once
#include "Shader.h"
#include "PostProcessing.h"

class HDR: private PostProcessing
{
	unsigned int TextureID;
	unsigned int FramebufferID;
	Shader shader;
	unsigned int RenderbufferID;
	unsigned int BloomTextureID;
	unsigned int PingPongBuffers[2] = { 0, 0 };
	unsigned int pingpongFBO[2] = { 0, 0 };
	Shader pingPongShader;

public: 
	HDR(int SCR_WIDTH, int SCR_HEIGHT);
	HDR() :TextureID(0), FramebufferID(0), RenderbufferID(0), BloomTextureID(0){};
	void bindFrameBuffer() const;
	void unbindFrameBuffer();
};

