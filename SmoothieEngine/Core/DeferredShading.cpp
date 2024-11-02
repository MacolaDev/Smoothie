#include "DeferredShading.h"
#include "GL/glew.h"
#include "Skybox.h"
#include "SSAO.h"
unsigned int DeferredShading::gBuffer = 0;
unsigned int DeferredShading::TempRenderBuffer = 0;

unsigned int DeferredShading::gPosition = 0;
unsigned int DeferredShading::gNormal = 0;
unsigned int DeferredShading::gAlbedoSpec = 0;
unsigned int DeferredShading::gMRAO = 0;

Shader DeferredShading::gBufferShader;
Shader DeferredShading::lightingPassShader;

void DeferredShading::generateBuffersTextures()
{
	//TODO: fix all of this OpenGL textrutes stuff with Texture class
	initPostProcessing();
	glewInit();
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// - position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SmoothieCore::SCR_WIDTH, SmoothieCore::SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	
	// - normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SmoothieCore::SCR_WIDTH, SmoothieCore::SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	
	// - color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SmoothieCore::SCR_WIDTH, SmoothieCore::SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	

	//PBR stuff
	glGenTextures(1, &gMRAO);
	glBindTexture(GL_TEXTURE_2D, gMRAO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SmoothieCore::SCR_WIDTH, SmoothieCore::SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMRAO, 0);


	// - tell OpenGL which color attachments we’ll use (of this framebuffer)
	unsigned int attachments[4] = { 
	GL_COLOR_ATTACHMENT0, 
	GL_COLOR_ATTACHMENT1, 
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3
	};
	glDrawBuffers(4, attachments);
	

	lightingPassShader = Shader("shaders/standard/pbr_render.glsl");

	lightingPassShader.use();
	lightingPassShader.setInt("gPosition", 0);
	lightingPassShader.setInt("gNormal", 1);
	lightingPassShader.setInt("gAlbedoSpec", 2);
	lightingPassShader.setInt("gMRAO", 3);

	lightingPassShader.setInt("irradianceMap", 4);
	lightingPassShader.setInt("prefilterMap", 5);
	lightingPassShader.setInt("brdfLUT", 6);
	lightingPassShader.setInt("dynamicSSA0", 7);


	glGenRenderbuffers(1, &TempRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, TempRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SmoothieCore::SCR_WIDTH, SmoothieCore::SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, TempRenderBuffer);
}

void DeferredShading::initilize()
{
	generateBuffersTextures();
}

void DeferredShading::bindGBuffer()
{
	glBindRenderbuffer(GL_RENDERBUFFER, TempRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0); // black so it won’t leak in g-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void DeferredShading::renderPBR()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gMRAO);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::irradianceMap);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::prefilterMap);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, Skybox::brdfLUTTexture);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, SSAO::BluredTexture.ID);

	lightingPassShader.use();
	
	RenderQuad();

	//Copying depth-map from gBuffer to default framebuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(0, 0, SmoothieCore::SCR_WIDTH, SmoothieCore::SCR_HEIGHT, 0, 0, SmoothieCore::SCR_WIDTH, SmoothieCore::SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
