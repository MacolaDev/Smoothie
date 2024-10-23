#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "VertexBuffer.h"
class Mesh
{
	std::vector<xyznuvtb> xyznuvtbBuffer;
	std::vector<unsigned int> indexBufferFromFile;
public:

	unsigned int* indexBufferData() {
		auto location = indexBufferFromFile.data();
		return location;
	}
	xyznuvtb* dataPointer() 
	{
		return xyznuvtbBuffer.data();
	}

	VertexBufferType type;
	int numberOfIndices;
	int numberOfvertices;

	int sizeOfIndexBuffer;
	int sizeOfVertexBuffer;

	Mesh(std::string filepath);
	Mesh() :numberOfIndices(-1), numberOfvertices(-1), sizeOfIndexBuffer(-1), sizeOfVertexBuffer(-1){};
	

};


