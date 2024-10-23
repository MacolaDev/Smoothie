#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>

void Texture::bindToShader(Shader& shader, const std::string& sampler, int location)
{
	shader.setInt(sampler, location);
	int err = glGetError();
	if (err != 0) std::cout << __FUNCTION__": error" << err << std::endl;
}

void Texture::bind(int id) const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, ID);
	int err = glGetError();
	if (err != 0) std::cout << __FUNCTION__": error" << err << std::endl;
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	int err = glGetError();
	if (err != 0) std::cout << __FUNCTION__": error" << err << std::endl;
}

Texture::Texture(const std::string& file)
{
	stbi_set_flip_vertically_on_load(true);

	dataFromFile = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
	
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	//set to GL_CLAMP_TO_EDGE for alpha textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (dataFromFile == nullptr) 
	{
		std::cout << __FUNCTION__": Can't load texture " + file + "." << std::endl;
	}

	if (nrChannels == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, dataFromFile);
	}
	if (nrChannels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataFromFile);
	}
	if (nrChannels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataFromFile);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(dataFromFile);
}

Texture::~Texture()
{
//	glDeleteTextures(1, &ID);
}
