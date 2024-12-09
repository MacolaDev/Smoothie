#include "IndexBuffer.h"
#include "SmoothieDebug.h"
#include <iostream>
#include <GL/glew.h>
IndexBuffer::IndexBuffer(unsigned int* data, int size)
{
	IndexData = data;
	sizeOfData = size;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(int), data, GL_STATIC_DRAW);
	SmoothieDebug(&ID);
	if (glGetError() != 0) std::cout << __FUNCTION__" error" << std::endl;
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	if (glGetError() != 0) std::cout << __FUNCTION__" error" << std::endl;
}
