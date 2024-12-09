#include "SmoothieCore.h"
#include <iostream>
#include "DeferredShading.h"
#include "GL/glew.h"
#include "SSAO.h"
#include "Effects/SSR.h"

int SmoothieCore::SCR_WIDTH = 1280;
int SmoothieCore::SCR_HEIGHT = 720;
float SmoothieCore::aspect = 1;//float(1280)/float(720);
bool SmoothieCore::isEngineReady = false;

float SmoothieCore::time = 0.01f;

Smoothie::Camera* SmoothieCore::camera = nullptr;
Smoothie::Scene* SmoothieCore::currentScene = nullptr;
Smoothie::UniformBuffers SmoothieCore::standardBuffer;
Smoothie::UniformBuffers SmoothieCore::additionalBuffer;
int TIME = 1;

Scene* SmoothieCore::getSceneInstance()
{
	return currentScene;
}
void SmoothieCore::updateRenderResolution(int width, int height)
{
	if ((width == 0) or (height == 0))
	{
		isEngineReady = false;
		return;
	}
	if ((width == SCR_WIDTH) and (height == SCR_HEIGHT))
	{
		isEngineReady = true;
		return;
	}
	else 
	{
		isEngineReady = true;
		SCR_WIDTH = width;
		SCR_HEIGHT = height;
		aspect = float(SCR_WIDTH) / float(SCR_HEIGHT);
		
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		DeferredShading::generateBuffersTextures();
		SSAO::generateSSAOTextures(SCR_WIDTH, SCR_HEIGHT);
		SSR::generateSSRTextures(SCR_WIDTH, SCR_HEIGHT);
		camera->updateProjectionMatrix(camera->fovy, aspect, camera->zNear, camera->zFar);
	}
}

void SmoothieCore::initilize()
{
	glewInit();
	isEngineReady = true;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	DeferredShading::generateBuffersTextures();

	standardBuffer.generateBuffer();
	standardBuffer.addBufferSubData(sizeof(Matrix4x4), camera->projectionMatrix.dataPointer());
	standardBuffer.addBufferSubData(sizeof(Matrix4x4), camera->cameraMatrix.dataPointer());
	standardBuffer.addBufferSubData(sizeof(Vector3), &camera->cameraPos);
	standardBuffer.addBufferSubData(sizeof(Matrix4x4), &camera->projectionViewMatrix);

	additionalBuffer.generateBuffer();
	additionalBuffer.addBufferSubData(sizeof(int), &SCR_WIDTH);
	additionalBuffer.addBufferSubData(sizeof(int), &SCR_HEIGHT);
	additionalBuffer.addBufferSubData(sizeof(Vector4), &TIME);

	SSAO::generateSSAOTextures(SCR_WIDTH, SCR_HEIGHT);
	SSR::generateSSRTextures(SCR_WIDTH, SCR_HEIGHT);
}

void SmoothieCore::render()
{
	if (isEngineReady == false) return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_CULL_FACE);

	standardBuffer.onRender();
	additionalBuffer.onRender();
	TIME++;

	camera->updateCameraMatrix();

	DeferredShading::bindGBuffer();

	for (int i = 0; i < currentScene->gBufferModels.size(); i++)
	{
		currentScene->gBufferModels[i].onRender();
	}

	SSAO::onRender();
	//SSR::onRender();

	DeferredShading::renderPBR();

	currentScene->skybox.drawSkybox();

	for (int i = 0; i < currentScene->lightPassModels.size(); i++)
	{
		currentScene->lightPassModels[i].onRender();
	}
	
	DeferredShading::finilize();
}

void SmoothieCore::useCamera(Smoothie::Camera& othercamera)
{
	camera = &othercamera;
}

void SmoothieCore::useScene(Smoothie::Scene* newScene)
{
	currentScene = newScene;
}

void SmoothieCore::GlDebugOutputSynchronous()
{
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
