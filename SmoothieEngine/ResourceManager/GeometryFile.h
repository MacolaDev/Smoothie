#pragma once
#include <string>
#include <vector>
#include <memory>
#include "VertexBuffer.h"
class GeometryFile
{
protected:
	std::string filepath;
	std::vector<char> vertexData;
	std::vector<char> indexData;

	unsigned int numberOfVertices;
	unsigned int numberOfIndices;
	unsigned int vertexTypeLenght;

	std::shared_ptr<VertexBufferBase> vertexBufferBasePtr;
	
	unsigned int vertexBufferType;
	unsigned int indexBufferType;


public:
	GeometryFile(const std::string& filepath);
	GeometryFile() = default;
};

