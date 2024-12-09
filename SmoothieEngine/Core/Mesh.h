#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "VertexBuffer.h"
#include "GeometryFile.h"
#include "ResourceHandler.h"

class Mesh: private GeometryFile, private ResourceHandler<Mesh>
{
	unsigned int VertexAttributeObject;
	unsigned int VertexBufferObject;
	unsigned int IndexBufferObject;

	bool isInitilized;
public:

	Mesh(std::string filepath, bool initilizeOpenGLFucntions = true);
	Mesh() = default;
	
	void generateOpenGLdata();

	void onRender();
};


