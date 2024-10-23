#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Shader.h"

#include "ParseXML.h"
#include "Vector3.h"
#include "Texture.h"
#include "Matrix4x4.h"

using namespace SmoothieMath;

namespace Smoothie {
	class Model
	{
	protected:

		ParseXML modelfile;
		std::string meshFile;

		Mesh mesh;
		unsigned int VertexAttributePointer;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Texture textures[16];
		std::string texturePropertyNames[16];
		unsigned int additionalTextures[16];

	public:
		Matrix4x4 modelMatrix;
		Model(const std::string& smFilePath);
		int textureCount;
		int additionalTexturesCount;
		Model() :VertexAttributePointer(-1), textureCount(0) {};

		//Adds new textures to shader.
		void addTexture(const std::string& name, unsigned int textureID);
		Shader shader;

		virtual void onRender();

		void onDeferredRender();

		//Only draws object with no shader bind to it. Used in shadow calculations.
		void drawNoShader() const;
	};
}

