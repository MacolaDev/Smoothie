#pragma once
#include "Scene.h"
#include "Camera.h"
#include "UniformBuffers.h"

class SmoothieCore
{
	static float aspect;
	static bool isEngineReady;
	static float time;
	//Camera that Smoothie uses
	static Smoothie::Camera* camera;

	//Scene that is being rendered
	static Smoothie::Scene* currentScene;

	//Standard uniform buffer that smoothie uses
	static Smoothie::UniformBuffers standardBuffer;
	
	static Smoothie::UniformBuffers additionalBuffer;

public:
	static int SCR_WIDTH, SCR_HEIGHT;

	//Returns instance of currently loaded scene
	static Smoothie::Scene* getSceneInstance();

	//Updates engine with new width and height.
	//This should be put inside render loop of your application. 
	static void updateRenderResolution(int SCR_WIDTH, int SCR_HEIGHT);

	//Renders active scene to screen.
	static void render();

	//Must be called before render loop.  
	static void initilize();

	//Tells engine on what camera should be used.
	//Must be set.
	static void useCamera(Smoothie::Camera& camera);
	
	//Active scene that Smoothie renders. Must be set.
	static void useScene(Smoothie::Scene* scene);

	static void GlDebugOutputSynchronous();
	
};