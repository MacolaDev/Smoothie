#include "VertexBuffer.h"
#include <iostream>
#include <GL/glew.h>

constexpr int fSize = (int)sizeof(float);
constexpr int iSize = (int)sizeof(unsigned int);

VertexBuffer::VertexBuffer(xyznuvtb* data, int size)
{
	glewInit();
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(xyznuvtb), data, GL_STATIC_DRAW);
	
}

VertexBuffer::VertexBuffer(xyznuvtbc* data, int size)
{
	glewInit();
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(xyznuvtbc), data, GL_STATIC_DRAW);
}


VertexBuffer::VertexBuffer(float* data, int size)
{
	glewInit();
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(void* data, unsigned long long size, int nOfElements)
{
	glewInit();
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, nOfElements * size, data, GL_STATIC_DRAW);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	if (glGetError() != 0) std::cout << __FUNCTION__" error" << std::endl;
}

unsigned int xyznuvtb::generateAttributeObject()
{
	unsigned int ID;
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	constexpr int size =
		3 * sizeof(float) //xyz
		+ 1 * (sizeof(unsigned int)) //n 
		+ 2 * (sizeof(float)) //uv
		+ 2 * (sizeof(unsigned int)); //tb


	//vertex coordinates, 3 float values
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
	glEnableVertexAttribArray(0);
	//vertex normals, 1 unsigned int value
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, size, (void*)(3 * fSize));
	glEnableVertexAttribArray(1);

	//vertex uv, 2 float values
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)(3 * fSize + iSize));
	glEnableVertexAttribArray(2);

	//tangent, 1 unsigned int value
	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, size, (void*)(5 * fSize + iSize));
	glEnableVertexAttribArray(3);

	//bitangent, 1 unsigned int value
	glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_FALSE, size, (void*)(5 * fSize + 2 * iSize));
	glEnableVertexAttribArray(4);
	return ID;
}

unsigned int xyznuvtbc::generateAttributeObject()
{
	unsigned int ID;
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	constexpr int size =
		3 * sizeof(float) //xyz
		+ 1 * (sizeof(unsigned int)) //n 
		+ 2 * (sizeof(float)) //uv
		+ 2 * (sizeof(unsigned int)) //tb
		+ 1 * (sizeof(unsigned int)); //color

	//vertex coordinates, 3 float values
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
	glEnableVertexAttribArray(0);

	//vertex normals, 1 unsigned int value
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, size, (void*)(3 * fSize));
	glEnableVertexAttribArray(1);
	
	//vertex uv, 2 float values
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)(3 * fSize + iSize));
	glEnableVertexAttribArray(2);

	//tangent, 1 unsigned int value
	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, size, (void*)(5 * fSize + iSize));
	glEnableVertexAttribArray(3);

	//bitangent, 1 unsigned int value
	glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_FALSE, size, (void*)(5 * fSize + 2 * iSize));
	glEnableVertexAttribArray(4);

	//color, 1 unsigned int value
	glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_FALSE, size, (void*)(5 * fSize + 3 * iSize));
	glEnableVertexAttribArray(5);
	return ID;
}
