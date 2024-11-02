#pragma once
#include <string>
#include "Shader.h"
namespace Smoothie {

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
	
	//Formats of image texture.
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

	class Texture
	{
		void* data;
		int width, height, nrChannels;
	public:
		unsigned int ID;
		
		void bindToShader(Shader& shader, const std::string& sampler, int location);
		
		void bind(int active_texture) const;
		
		void unbind() const;

		Texture(const std::string& name);

		
		Texture(unsigned char* data, int width, int height, Smoothie::TextureFormat format = TextureFormat::RGB, TextureInternalFormatUCHAR internalformat = TextureInternalFormatUCHAR::RGB8);

		//Texture constructor for floating point values
		Texture(float* data, int width, int height, Smoothie::TextureFormat format = TextureFormat::RGB, TextureInternalFormatFloat internalformat = TextureInternalFormatFloat::RGB32);
		
		//Sets texture wrapping.
		void setWrapping(TextureWrapping wrapping = TextureWrapping::REPEAT) const;
		
		//Sets textrue filtering.
		void setFiltering(TextureFiltering filtering = TextureFiltering::LINEAR) const;
		
		//Binds this texture as color attachment to a framebuffer
		void bindToFramebuffer(unsigned int bramebufferID, unsigned int colorAttachment) const;

		Texture() = default;
	};

}