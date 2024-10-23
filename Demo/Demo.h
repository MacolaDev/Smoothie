#pragma once
#include "SmoothieCore.h"
#include "glfw3.h"

void processKeyboardEvent(GLFWwindow* window, Smoothie::Camera& camera);
void windowChagneSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);

class Demo 
{
	GLFWwindow* window = nullptr;
public:
	
	Demo();
	~Demo();
	void render();

};

