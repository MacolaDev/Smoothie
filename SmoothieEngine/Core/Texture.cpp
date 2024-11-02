#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>

using namespace Smoothie;
void Texture::bindToShader(Shader& shader, const std::string& sampler, int location)
{
	shader.setInt(sampler, location);
}

void Texture::bind(int id) const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string& file)
{
	stbi_set_flip_vertically_on_load(true);

	data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
	
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	
	//set to GL_CLAMP_TO_EDGE for alpha textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (data == nullptr) 
	{
		std::cout << __FUNCTION__": Can't load texture " + file + "." << std::endl;
	}

	if (nrChannels == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	}
	if (nrChannels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	if (nrChannels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void Smoothie::Texture::setWrapping(TextureWrapping wrapping) const
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
}

void Smoothie::Texture::setFiltering(TextureFiltering filtering) const
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
}

void Smoothie::Texture::bindToFramebuffer(unsigned int framebufferID, unsigned int colorAttachment) const
{
	//TODO: Add range checker if colorAttachment is greater then supported one
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachment, GL_TEXTURE_2D, ID, 0);
}

Texture::Texture(unsigned char* data, int width, int height, TextureFormat format, TextureInternalFormatUCHAR internalformat)
{
	this->data = data;
	this->width = width;
	this->height = height;
	
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	
	setWrapping();
	setFiltering();
}

Smoothie::Texture::Texture(float* data, int width, int height, Smoothie::TextureFormat format, TextureInternalFormatFloat internalformat)
{
	this->data = data;
	this->width = width;
	this->height = height;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_FLOAT, data);

	setWrapping();
	setFiltering();
}

