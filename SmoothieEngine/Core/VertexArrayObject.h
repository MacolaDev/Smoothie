#pragma once
#include "VertexBuffer.h"

class VertexArrayObject
{
public:
	static void GenerateVertexArray(unsigned int ID, VertexBufferType bufferType);
	
	//All static models should use this 
	static void VertexAttributePointerXYZNUVTB(unsigned int ID);
	
	//Mostly used for planes in post-processing
	static void VertexAttributePointerXYUV(unsigned int ID);
};

