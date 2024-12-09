#include "SSR.h"
#include <GL/glew.h>
unsigned int SSR::fbo = 0;
unsigned int SSR::fboblured = 0;
Shader SSR::ssrshader;
InternalTexture2D SSR::target;
InternalTexture2D SSR::bluredTarget;
Shader SSR::bluredshader;


void SSR::generateSSRTextures(int SCR_WIDTH, int SCR_HEIGHT)
{
	initPostProcessing();
	target = InternalTexture2D((float*)nullptr, SCR_WIDTH, SCR_HEIGHT, RGB, RGB32);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	target.bindToFramebuffer(fbo, 0);

	ssrshader = Shader("shaders/standard/SSR.glsl");
	ssrshader.use();
	ssrshader.setInt("gPosition", 0);
	ssrshader.setInt("gNormal", 1);
	ssrshader.setInt("gMRAO", 2);
	ssrshader.setInt("gAlbedoSpec", 3);

	//Bluring
	bluredTarget = InternalTexture2D((float*)nullptr, SCR_WIDTH, SCR_HEIGHT, RGB, RGB32);
	glGenFramebuffers(1, &fboblured);
	glBindFramebuffer(GL_FRAMEBUFFER, fboblured);
	bluredTarget.bindToFramebuffer(fboblured, 0);

	bluredshader = Shader("shaders/blure/ssrblure.glsl");
	bluredshader.use();
	bluredshader.setInt("ssr", 0);

}

void SSR::onRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ssrshader.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DeferredShading::gPosition);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, DeferredShading::gNormal);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, DeferredShading::gMRAO);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, DeferredShading::gAlbedoSpec);
	
	RenderQuad();

	//Blure ssr texture
	glBindFramebuffer(GL_FRAMEBUFFER, fboblured);

	bluredshader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, target.ID);
	
	RenderQuad();
}
