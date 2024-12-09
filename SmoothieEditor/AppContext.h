#pragma once
#include <iostream>
#include "imgui.h"
#include "glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "SmoothieCore.h"
#include "SmoothieMath.h"

struct AppContext
{
	static int width, height;
	static float aspect_ratio;

	static Smoothie::Scene loadedScene;
	static SmoothieMath::Vector3 cameraPosition;
	static Smoothie::Camera camera;

	static float cameraSpeed, sensitivity, deltaTime;

	static Smoothie::Model* selectedModel;
};

//Updates render engine and app context with new viewport width and height
void windowChagneSizeCallback(GLFWwindow* window, int width, int height);

void processKeyboardEvent(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);


void SmoothieDebugData();

void SceneGraph();

void ModelEditor();

void CameraInfo();