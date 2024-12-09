#include "GeometryFile.h"
#include <fstream>
static inline int getIntFromFile(std::ifstream& file) {
	char buffer[4];
	file.read(buffer, 4);
	int value = *(int*)buffer;
	return value;
}


GeometryFile::GeometryFile(const std::string& filepath) : indexBufferType(0)
{
	this->filepath = filepath;
	auto file = std::ifstream(filepath, std::ios_base::binary);

	vertexBufferType = getIntFromFile(file);
	numberOfVertices = getIntFromFile(file);
	int vertexOffset = getIntFromFile(file);

	numberOfIndices = getIntFromFile(file);
	int indexOffset = getIntFromFile(file);
	
	
	vertexBufferBasePtr = std::make_shared<VertexBufferBase>();

	if (vertexBufferType == 0) 
	{
		vertexTypeLenght = sizeof(xyznuvtb) - sizeof(VertexBufferBase);
		vertexBufferBasePtr = std::make_shared<xyznuvtb>();
	}
	if (vertexBufferType == 1)
	{
		vertexTypeLenght = sizeof(xyznuvtbc) - sizeof(VertexBufferBase);
		vertexBufferBasePtr = std::make_shared<xyznuvtbc>();
	}

	file.seekg(vertexOffset);

	for(int i = 0; i < numberOfVertices * vertexTypeLenght; i++)
	{
		vertexData.push_back(file.get());
	}
		
	file.seekg(indexOffset);
	for (int i = 0; i < numberOfIndices * sizeof(unsigned int); i++)
	{
		indexData.push_back(file.get());
	}

	file.close();
}
