#include "SmoothieCore.h"
#include <iostream>
#include "DeferredShading.h"
#include "GL/glew.h"
#include "SSAO.h"
int SmoothieCore::SCR_WIDTH = 1280;
int SmoothieCore::SCR_HEIGHT = 720;
float SmoothieCore::aspect = 1;//float(1280)/float(720);
bool SmoothieCore::isEngineReady = false;

Smoothie::Camera* SmoothieCore::camera = nullptr;
Smoothie::Scene SmoothieCore::currentScene;
Smoothie::UniformBuffers SmoothieCore::standardBuffer;

void SmoothieCore::updateRenderResolution(int width, int height)
{
	if ((width == SCR_WIDTH) and (height == SCR_HEIGHT))
	{
		return;
	}
	else 
	{
		SCR_WIDTH = width;
		SCR_HEIGHT = height;
		aspect = float(SCR_WIDTH) / float(SCR_HEIGHT);
		
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		DeferredShading::generateBuffersTextures();
		SSAO::generateSSAOTextures(SCR_WIDTH, SCR_HEIGHT);
		camera->updateProjectionMatrix(camera->fovy, aspect, camera->zNear, camera->zFar);
	}
}

void SmoothieCore::render()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	standardBuffer.onRender();

	camera->updateCameraMatrix();

	DeferredShading::bindGBuffer();

	for (int i = 0; i < currentScene.pbrSceneModels.size(); i++)
	{
		currentScene.pbrSceneModels[i].onRender();
	}

	SSAO::onRender();

	DeferredShading::renderPBR();

	currentScene.skybox.drawSkybox();
	for (int i = 0; i < currentScene.OtherSceneModels.size(); i++)
	{
		currentScene.OtherSceneModels[i].onRender();
	}
}

void SmoothieCore::initilize()
{
	glewInit();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	DeferredShading::generateBuffersTextures();

	standardBuffer.generateBuffer();
	standardBuffer.addBufferSubData(sizeof(Matrix4x4), camera->projectionMatrix.dataPointer());
	standardBuffer.addBufferSubData(sizeof(Matrix4x4), camera->cameraMatrix.dataPointer());
	standardBuffer.addBufferSubData(sizeof(Vector3), &camera->cameraPos);

	SSAO::generateSSAOTextures(SCR_WIDTH, SCR_HEIGHT);
}

void SmoothieCore::useCamera(Smoothie::Camera& othercamera)
{
	camera = &othercamera;
}

void SmoothieCore::useScene( const Smoothie::Scene& newScene)
{
	currentScene = newScene;
}

void SmoothieCore::GlDebugOutputSynchronous()
{
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
