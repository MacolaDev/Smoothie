#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>

using namespace Smoothie;

Texture::Texture(const std::string& file)
{
	//File is used as identifier of a texture.
	//If texture like that exists, we only want to save the name of the file without copying everything else in memory
	//When this texture gets bined, resourse handler will use file name to find that texture in map and bind that texture. 
	this->file = file; 
	if (loadedTextures.isAlreadyLoaded(file)) 
	{
		Texture* original = loadedTextures.getResource(file);
		this->data = original->data;
		this->width = original->width;
		this->height = original->height;
		this->nrChannels = original->nrChannels;
		this->ID = original->ID;
		return;
	};
	
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

	loadedTextures.addResource(file, *this); 
}

void Texture::bind(int active) const
{
	glActiveTexture(GL_TEXTURE0 + active);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void BaseTexture::bindToShader(Shader& shader, const std::string& sampler, int location)
{
	shader.setInt(sampler, location);
}

void BaseTexture::bind(int active_texture) const
{
	glActiveTexture(GL_TEXTURE0 + active_texture);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void BaseTexture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void BaseTexture::bindToFramebuffer(unsigned int framebufferID, unsigned int colorAttachment) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachment, GL_TEXTURE_2D, ID, 0);
}

InternalTexture2D::InternalTexture2D(float* data, int width, int height, TextureFormat format, TextureInternalFormatFloat internalformat)
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

InternalTexture2D::InternalTexture2D(unsigned char* data, int width, int height, TextureFormat format, TextureInternalFormatUCHAR internalformat)
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

void InternalTexture2D::setWrapping(TextureWrapping wrapping) const
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
}

void InternalTexture2D::setFiltering(TextureFiltering filtering) const
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
}
