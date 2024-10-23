#pragma once

enum VertexBufferType {
	XYZNUVTB,
	XYZNUVIIIWWTB
};

struct xyznuvtb
{
	float x, y, z;
	unsigned int n;
	float u, v;
	unsigned int t, b;
	xyznuvtb(char* data);
};

class VertexBuffer
{
	
public:
	unsigned int ID;
	VertexBuffer(xyznuvtb* data, int size);
	VertexBuffer(float* data, int size);
	void bind() const;
	VertexBuffer() = default;
};

