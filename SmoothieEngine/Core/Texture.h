#pragma once
#include <string>
#include "Shader.h"
#include <string>

class Texture
{
	unsigned char* dataFromFile;
	int width, height, nrChannels;
public:
	unsigned int ID;
	void bindToShader(Shader& shader, const std::string& sampler, int location);
	void bind(int id) const;
	void unbind() const;
	Texture(const std::string& name);
	Texture() :ID(0), dataFromFile(nullptr), width(0), height(0), nrChannels(0) {};
	~Texture();
};

