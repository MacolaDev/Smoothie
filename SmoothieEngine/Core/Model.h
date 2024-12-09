#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Shader.h"

#include "ParseXML.h"
#include "Texture.h"
#include "SmoothieMath.h"
#include "BoundBox.h"

namespace Smoothie 
{
	class Scene;
	class Model
	{
	protected:
		BoundBox boundBox;
		ParseXML modelFileXML;
		std::string meshFile;

		std::string modelFilePath;
		
		Mesh mesh;

		Texture textures[16];
		
		bool doubleSided;
		bool blending;
		int textureCount;

		std::string texturePropertyNames[16];

		SmoothieMath::Vector3 modelPosition;
		SmoothieMath::Vector3 modelScale;
		SmoothieMath::Vector3 modelRotation;

	public:
		friend class Scene;
		Matrix3x3 normalMatrix;
		Matrix4x4 modelMatrix;

		Model(const std::string& smFilePath);
		Model() = default;

		Shader shader;

		void onRender();

		//Only draws object with no shader bind to it. Used in shadow calculations.
		void drawNoShader() const;
		
		//Draws bounding box of a model.
		void drawBoundBox();
		
		void setModelScale(const SmoothieMath::Vector3& scale);
		
		SmoothieMath::Vector3 getModelScale() const;

		void setModelPosition(const SmoothieMath::Vector3& position);
		
		SmoothieMath::Vector3 getModelPosition() const;

		void setModelRotationEuler(const SmoothieMath::Vector3& euler_angles);
		
		SmoothieMath::Vector3 getModelRotationEuler() const;

		void updateModelTransformMatrix(
			const SmoothieMath::Vector3& translation, 
			const SmoothieMath::Vector3& rotationEuler,
			const SmoothieMath::Vector3& scale);

		void updateModelTransformMatrix();
	};
}

