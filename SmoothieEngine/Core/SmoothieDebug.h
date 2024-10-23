#pragma once
#include "GL/glew.h"
#include <glfw3.h>
#include <iostream>
void APIENTRY GLMsgCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* msg, const void* data);
#define SmoothieDebug( id) glDebugMessageCallback(GLMsgCallback, id)
