#include "ShadowMaps.h"
#include "GL/glew.h"
#include "PostProcessing.h"
#include "SmoothieDebug.h"
ShadowMaps::ShadowMaps(int imageWidth, int imageHight, int screenWidth, int screenHeight)
{
	this->imageWidth = imageWidth;
	this->imageHight = imageHight;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	//Depth map 
	glewInit();
	glGenFramebuffers(1, &depthMapFB);
	SmoothieDebug(&depthMapFB);

	glGenTextures(1, &depthMapTexture);
	SmoothieDebug(&depthMapTexture);

	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, imageWidth, imageHight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

	//Telling opengl that we dont care about color values
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader = Shader("shaders/shadowMapVertex.glsl", "shaders/shadowMapFragment.glsl");


	//Lighting stuff
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(
		glm::vec3(-4.0f, 4.0f, -1.0f), //Light position
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	lightSpaceMatrix = lightProjection * lightView;
}

void ShadowMaps::firstPass()
{
	
	shader.use();
	shader.setMatrix4x4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFB);
	glViewport(0, 0, imageWidth, imageHight);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	RenderQuad();
}

void ShadowMaps::secondPass() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
}

float* ShadowMaps::lightSpaceMatrixPtr()
{
	return glm::value_ptr(lightSpaceMatrix);
}
