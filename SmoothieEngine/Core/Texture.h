#pragma once
#include <string>
#include "Shader.h"
#include "ResourceHandler.h"


//Base texture class for all textures in Smoothie Engine
class BaseTexture 
{
protected:
	void* data;
	int width, height, nrChannels;


public:
	BaseTexture() = default;
	
	unsigned int ID;

	virtual void bindToShader(Shader& shader, const std::string& sampler, int location);

	virtual void bind(int active_texture) const;

	void unbind();

	void bindToFramebuffer(unsigned int bramebufferID, unsigned int colorAttachment) const;
};

//Formats of image texture.
enum TextureWrapping
{
	REPEAT = 0x2901, 
	MIRRORED_REPEAT = 0x8370,
	CLAMP_TO_EDGE = 0x812F,
	CLAMP_TO_BORDER = 0x812D

};

enum TextureFiltering
{
	NEARES = 0x2600,
	LINEAR = 0x2601
};

enum TextureFormat
{
	R = 0x1903,
	RG = 0x8227,
	RGB = 0x1907,
	RGBA = 0x1908
};

//Internal texture formats for unsigned char
enum TextureInternalFormatUCHAR
{
	R8 = 0x1903,
	RG8 = 0x8227,
	RGB8 = 0x1907,
	RGBA8 = 0x1908
};

//Internal texture formats for 16-bit and 32-bitfloat data
enum TextureInternalFormatFloat
{
	R16 = 0x822D,
	RG16 = 0x822F,
	RGB16 = 0x881B,
	RGBA16 = 0x881A,
	R32 = 0x822E,
	RG32 = 0x8230,
	RGB32 = 0x8815,
	RGBA32 = 0x8814,
};


namespace Smoothie 
{
	class Texture: public BaseTexture
	{
		ResourceHandler<Texture> loadedTextures;

		std::string file;
	public:

		Texture(const std::string& name);

		void bind(int active_texture) const override;
			
		Texture() = default;
	};

}

//This is only used for textures that Smoothie uses internaly
class InternalTexture2D: public BaseTexture
{
public:

	InternalTexture2D(float* data, int width, int height, TextureFormat format = TextureFormat::RGB, TextureInternalFormatFloat internalformat = TextureInternalFormatFloat::RGB32);
	
	InternalTexture2D(unsigned char* data, int width, int height, TextureFormat format = TextureFormat::RGB, TextureInternalFormatUCHAR internalformat = TextureInternalFormatUCHAR::RGB8);

	//Sets texture wrapping.
	void setWrapping(TextureWrapping wrapping = TextureWrapping::REPEAT) const;

	//Sets textrue filtering.
	void setFiltering(TextureFiltering filtering = TextureFiltering::LINEAR) const;
	
	InternalTexture2D() = default;
};