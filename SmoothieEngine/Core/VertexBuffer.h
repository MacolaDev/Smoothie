#pragma once

enum VertexBufferType {
	XYZNUVTB = 0,
	XYZNUVTBC = 1
};

class VertexBufferBase 
{
public:
	virtual unsigned int generateAttributeObject() { return 0; };
	VertexBufferBase() = default;
};

struct xyznuvtb : public VertexBufferBase
{
	float x, y, z;
	unsigned int n;
	float u, v;
	unsigned int t, b;
	unsigned int generateAttributeObject() override;
	xyznuvtb() = default;
};


struct xyznuvtbc : public VertexBufferBase
{
	float x, y, z;
	unsigned int n;
	float u, v;
	unsigned int t, b, color;
	unsigned int generateAttributeObject() override;
	xyznuvtbc() = default;
};

class VertexBuffer
{
	
public:
	unsigned int ID;
	VertexBuffer(xyznuvtb* data, int size);
	VertexBuffer(xyznuvtbc* data, int size);
	VertexBuffer(float* data, int size);
	VertexBuffer(void* data, unsigned long long size, int nOfElements);
	void bind() const;
	VertexBuffer() = default;
};

