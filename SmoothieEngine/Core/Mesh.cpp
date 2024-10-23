#include "Mesh.h"
#include <vector>
#include <fstream>
#include <string>
int getIntFromFile(std::ifstream& file) {
	char buffer[4];
	file.read(buffer, 4);
	int value = *(int*)buffer;
	return value;
}

int getUIntFromFile(std::ifstream& file) {
	char buffer[4];
	file.read(buffer, 4);
	int value = *(unsigned int*)buffer;
	return value;
}

float getFloatFromFile(std::ifstream& file) {
	char buffer[4];
	file.read(buffer, 4);
	float value = *(float*)buffer;
	return value;
}

Mesh::Mesh(std::string filepath)
{	
	auto file = std::ifstream(filepath, std::ios_base::binary);
	
	int vertexType = getIntFromFile(file);
	numberOfvertices = getIntFromFile(file);
	int vertexOffset = getIntFromFile(file);

	numberOfIndices = getIntFromFile(file);
	int indexOffset = getIntFromFile(file);
	
	file.seekg(vertexOffset);
	if (vertexType == VertexBufferType::XYZNUVTB) 
	{	
		type = VertexBufferType::XYZNUVTB;
		for (int i = 0; i < numberOfvertices; i++) {
			char vertex[sizeof(xyznuvtb)];
			file.read(vertex, sizeof(xyznuvtb));
			xyznuvtbBuffer.push_back(vertex);
		}
	}

	file.seekg(indexOffset);
	sizeOfIndexBuffer = numberOfIndices;
	for (int i = 0; i < sizeOfIndexBuffer; i++)
	{
		indexBufferFromFile.push_back(getUIntFromFile(file));
	}
	file.close();
}

