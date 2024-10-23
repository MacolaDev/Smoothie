#include "VertexBuffer.h"
#include <iostream>
#include <GL/glew.h>
VertexBuffer::VertexBuffer(xyznuvtb* data, int size)
{
	glewInit();
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(xyznuvtb), data, GL_STATIC_DRAW);
	
}


VertexBuffer::VertexBuffer(float* data, int size)
{
	glewInit();
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	if (glGetError() != 0) std::cout << __FUNCTION__" error" << std::endl;
}

xyznuvtb::xyznuvtb(char* data)
{

	x = *reinterpret_cast<float*>(&data[0]);
	y = *reinterpret_cast<float*>(&data[4]);
	z = *reinterpret_cast<float*>(&data[8]);

	n = *reinterpret_cast<int*>(&data[12]);

	u = *reinterpret_cast<float*>(&data[16]);
	v = *reinterpret_cast<float*>(&data[20]);

	t = *reinterpret_cast<int*>(&data[24]);
	b = *reinterpret_cast<int*>(&data[28]);

}

