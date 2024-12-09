#include "Model.h"
#include <fstream>
#include <sstream>
#include "ParseXML.h"
#include "Vector3.h"
#include "SmoothieDebug.h"
#include "VertexArrayObject.h"
#include "DeferredShading.h"
#include "GL/glew.h"

using namespace Smoothie;
using namespace SmoothieMath;

Model::Model(const std::string& smFilePath)
{
	modelFilePath = smFilePath;
	modelFileXML = ParseXML(smFilePath);	
	textureCount = 0;

	doubleSided = false;
	blending = false;
	modelScale = { 1.0f, 1.0f, 1.0f };

	glewInit();
	mesh = Mesh(modelFileXML.getElement("geometryFile").textContent);

	std::string shaderFile = modelFileXML.getElement("shader").textContent;
	

	shader = Shader(shaderFile);
	shader.use();


	Element propertiesElement = modelFileXML.getElement("property");
	for (int i = 0; i < propertiesElement.children.size(); i++)
	{
		Element shaderProperty = propertiesElement.children[i];
		std::string propertyType = shaderProperty.name;
		std::string propertyName = shaderProperty.textContent;
		std::string propertyValue = shaderProperty.children[0].textContent;

		if (propertyType == "Float") 
		{
			float value = shaderProperty.children[0].getFloat();
			shader.setFloat(propertyName, value);
		}

		if (propertyType == "Vector3") 
		{
			Vector3 value = shaderProperty.children[0].getVector3();
			shader.setVector3(propertyName, value);
		}

		if (propertyType == "Texture") 
		{
			textures[textureCount] = {propertyValue};
			textures[textureCount].bindToShader(shader, propertyName, textureCount);
			texturePropertyNames[textureCount] = propertyName;
			textureCount++;
		}

		if (propertyType == "Bool") 
		{
			if (propertyName == "doubleSided") 
			{
				doubleSided = shaderProperty.children[0].getBool();
			}

			if (propertyName == "useAlpha")
			{
				blending = shaderProperty.children[0].getBool();
			}
		}


	}
	boundBox = { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f} };

	Element BoundingBox = modelFileXML.getElement("BoundingBox");
	boundBox = BoundBox(BoundingBox.children[0].getVector3(), BoundingBox.children[1].getVector3());
}

void Model::onRender()
{

	//Enable/disable alpha
	if (blending) 
	{
		glEnable(GL_BLEND);
	}
	shader.use();


	//Rendering on both sides
	if (doubleSided)
	{
		glDisable(GL_CULL_FACE);
	}

	for (int i = 0; i < textureCount; i++) {
		textures[i].bind(i);
	}
	
	shader.setMatrix4x4("modelMatrix", modelMatrix.dataPointer());
	shader.setMatrix3x3("normalMatrix", normalMatrix.dataPointer());

	mesh.onRender();
	
	if (blending)
	{
		glDisable(GL_BLEND);
	}
	
	if (doubleSided)
	{
		glEnable(GL_CULL_FACE);
	}
}

void Model::drawNoShader() const
{
	/*vertexBuffer.bind();
	glBindVertexArray(VertexAttributePointer);
	glDrawElements(GL_TRIANGLES, indexBuffer.sizeOfData, GL_UNSIGNED_INT, 0);*/

}

void Smoothie::Model::drawBoundBox()
{
	boundBox.drawBoundBox(modelMatrix);
}

void Smoothie::Model::setModelScale(const SmoothieMath::Vector3& scale)
{
	modelScale = scale;
}

SmoothieMath::Vector3 Smoothie::Model::getModelScale() const
{
	return modelScale;
}

void Smoothie::Model::setModelPosition(const SmoothieMath::Vector3& position)
{
	modelPosition = position;
}

SmoothieMath::Vector3 Smoothie::Model::getModelPosition() const
{
	return modelPosition;
}

void Smoothie::Model::setModelRotationEuler(const SmoothieMath::Vector3& euler_angles)
{
	modelRotation = euler_angles;
}

SmoothieMath::Vector3 Smoothie::Model::getModelRotationEuler() const
{
	return modelRotation;
}

void Smoothie::Model::updateModelTransformMatrix(const SmoothieMath::Vector3& translation, const SmoothieMath::Vector3& rotationEuler, const SmoothieMath::Vector3& scale)
{
	modelMatrix.transformMatrix(translation, rotationEuler, scale);
	/*Matrix4x4 normalMatrix = Matrix4x4(modelMatrix);
	normalMatrix.inverse();
	normalMatrix.transpose();
	this->normalMatrix = Matrix3x3(normalMatrix);*/
	this->normalMatrix = modelMatrix.normalMatrix();
}

void Smoothie::Model::updateModelTransformMatrix()
{
	modelMatrix.transformMatrix(modelPosition, modelRotation, modelScale);
	/*Matrix4x4 normalMatrix = Matrix4x4(modelMatrix);
	normalMatrix.inverse();
	normalMatrix.transpose();
	this->normalMatrix = Matrix3x3(normalMatrix);
	modelMatrix.normalMatrix();*/
	this->normalMatrix = modelMatrix.normalMatrix();
}

