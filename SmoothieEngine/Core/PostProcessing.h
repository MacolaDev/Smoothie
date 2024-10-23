#pragma once
#include "VertexBuffer.h"
#include <iostream>

//This class is supposed to be used as a nice abstraction for drawing quads to screen.
//To use it, make your class inherit from this one and call RenderQuad() whenever you need to draw something.
class PostProcessing
{
	static unsigned int quatVertexAttribue;
	static VertexBuffer quad;
public:
	PostProcessing() = default;

	//Initilizes post processing. Should be called in constructor of child class
	static void initPostProcessing();
	
	//Draws a quad on you screen.
	//Dont forget to bind your shader before calling it. 
	static void RenderQuad();
};

