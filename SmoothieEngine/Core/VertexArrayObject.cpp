#include "VertexArrayObject.h"
#include "GL/glew.h"
#include "SmoothieDebug.h"

constexpr int fSize = (int)sizeof(float);
constexpr int iSize = (int)sizeof(unsigned int);
void VertexArrayObject::GenerateVertexArray(unsigned int ID, VertexBufferType bufferType)
{
	glewInit();
	glBindVertexArray(ID);
	if (bufferType == VertexBufferType::XYZNUVTB) 
	{
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

		return;
	}
	if (bufferType == VertexBufferType::XYZNUVTBC) 
	{
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
	}
}

void VertexArrayObject::VertexAttributePointerXYZNUVTB(unsigned int ID)
{
	glewInit();
	glBindVertexArray(ID);
	constexpr int size =
		  3 * sizeof(float) //xyz
		+ 1 * (sizeof(unsigned int)) //n 
		+ 2 * (sizeof(float)) //uv
		+ 2 * (sizeof(unsigned int)); //tb

	constexpr int fSize = (int)sizeof(float);
	constexpr int iSize = (int)sizeof(unsigned int);

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
}

void VertexArrayObject::VertexAttributePointerXYUV(unsigned int ID)
{

	glewInit();
	glBindVertexArray(ID);
	constexpr int size = 4 * sizeof(float);
	//xy, 2 float values
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, size, (void*)0);
	glEnableVertexAttribArray(0);
	
	//uv, 2 float values
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void VertexArrayObject::VertexAttributePointerXYZ(unsigned int ID)
{
	glewInit();
	glBindVertexArray(ID);
	constexpr int size = 8 * sizeof(float);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
	glEnableVertexAttribArray(0);

}
