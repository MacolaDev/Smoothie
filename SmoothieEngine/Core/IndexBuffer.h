#pragma once
class IndexBuffer
{
	unsigned int ID;
public:
	unsigned int* IndexData;
	int sizeOfData;
	IndexBuffer(unsigned int* data, int sizeOfData);
	void bind();
	explicit IndexBuffer() : ID(-1), IndexData(0), sizeOfData(0) {};
};

