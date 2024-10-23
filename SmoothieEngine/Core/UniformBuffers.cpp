#include "UniformBuffers.h"
#include "GL/glew.h"
#include <cmath>
using namespace Smoothie;
unsigned int UniformBuffers::UniformBuffersCount = 0;
unsigned int UniformBuffers::UniformBuffersOffset = 0;
std::vector<unsigned int> UniformBuffers::bufferRangeSizes;

static unsigned int getUniformBlockSize(const std::vector<UniformBufferData>& data)
{
	unsigned int size = 0;
	for (int i = 0; i < data.size(); i++)
	{
		size += data[i].dataSize;
	}
	return size;
}

void UniformBuffers::generateBuffer()
{

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	currentBufferIndex = UniformBuffersCount;
	UniformBuffersCount++;
	uniformBufferSize = 0;

	if (bufferRangeSizes.size() == 0)
	{
		bufferRangeSizes.push_back(0);
	}
}

void UniformBuffers::addBufferSubData(const UniformBufferData& data)
{
	bufferSubData.push_back(data);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	glBufferData(GL_UNIFORM_BUFFER, getUniformBlockSize(bufferSubData), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffers::addBufferSubData(unsigned int dataSize, void* dataPtr)
{
	//TODO: change this 32 bytes hardcoded
	unsigned int padding = 0;
	if ((dataSize % 16) != 0) 
	{
		padding = 16 - (dataSize % 16);
	}
	UniformBufferData data = { dataSize + padding, dataPtr };
	bufferSubData.push_back(data);

	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	glBufferData(GL_UNIFORM_BUFFER, getUniformBlockSize(bufferSubData), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffers::onRender()
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
	glBindBufferBase(GL_UNIFORM_BUFFER, currentBufferIndex, bufferID);
	unsigned int offset = 0;
	for (int i = 0; i < bufferSubData.size(); i++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, bufferSubData[i].dataSize, bufferSubData[i].dataPtr);
		offset += bufferSubData[i].dataSize;

	}
}
