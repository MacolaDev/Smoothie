#include "PostProcessing.h"
#include <string>
#include <iostream>
#include "GL/glew.h"
#include "glfw3.h"
#include "SmoothieDebug.h"
#include "VertexArrayObject.h"

VertexBuffer PostProcessing::quad;
unsigned int PostProcessing::quatVertexAttribue;

float quadVertices[24] = {
	// positions // texCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

void PostProcessing::initPostProcessing()
{
	quad = VertexBuffer(quadVertices, 24);
	glGenVertexArrays(1, &quatVertexAttribue);
	VertexArrayObject::VertexAttributePointerXYUV(quatVertexAttribue);
}

void PostProcessing::RenderQuad()
{
	glBindVertexArray(quatVertexAttribue);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
