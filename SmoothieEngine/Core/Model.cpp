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

float readFloat(const std::string& value)
{
	std::istringstream data(value);
	float x;
	data >> x;
	return x;
}

Vector3 readVector3(const std::string& value)
{
	Vector3 result;
	std::istringstream data(value);
	float x, y, z;
	data >> x >> y >> z;
	result.setX(x);
	result.setY(y);
	result.setZ(z);
	return result;
}

void Model::addTexture(const std::string& name, unsigned int textureID)
{
	if (textureCount + additionalTexturesCount >= 16) {
		std::cout << "Cant add new textures, maximum reached." << std::endl;
		return;
	}
	else {
		shader.use();
		glActiveTexture(GL_TEXTURE0 + textureCount + additionalTexturesCount);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader.setInt(name, textureCount + additionalTexturesCount);
		additionalTextures[additionalTexturesCount] = textureID;
		additionalTexturesCount++;
	}
}


Model::Model(const std::string& smFilePath)
{
	
	modelfile = ParseXML(smFilePath);	
	textureCount = 0;
	additionalTexturesCount = 0;

	glewInit();
	mesh = Mesh(modelfile.getElement("geometryFile").textContent);
	vertexBuffer = VertexBuffer(mesh.dataPointer(), mesh.numberOfvertices);
	indexBuffer = IndexBuffer(mesh.indexBufferData(), mesh.numberOfIndices);

	std::string vertexShaderFile;
	std::string fragmentShaderFile = modelfile.getElement("fragmentShader").textContent;
	
	if (mesh.type == VertexBufferType::XYZNUVTB)
	{
		glGenVertexArrays(1, &VertexAttributePointer);
		glBindVertexArray(VertexAttributePointer);
		VertexArrayObject::GenerateVertexArray(VertexAttributePointer, VertexBufferType::XYZNUVTB);
		vertexShaderFile = "shaders/formats/xyznuvtb.glsl";
	}


	shader = Shader(fragmentShaderFile);
	shader.use();


	Element propertiesElement = modelfile.getElement("property");
	for (int i = 0; i < propertiesElement.children.size(); i++)
	{
		Element shaderProperty = propertiesElement.children[i];
		std::string propertyType = shaderProperty.name;
		std::string propertyName = shaderProperty.textContent;
		std::string propertyValue = shaderProperty.children[0].textContent;

		if (propertyType == "Float") {
			float value = readFloat(propertyValue);
			shader.setFloat(propertyName, value);
		}
		if (propertyType == "Vector3") {
			Vector3 value = readVector3(propertyValue);
			shader.setVector3(propertyName, value);
		}
		if (propertyType == "Texture") {
			textures[textureCount] = {propertyValue};
			textures[textureCount].bindToShader(shader, propertyName, textureCount);
			texturePropertyNames[textureCount] = propertyName;
			textureCount++;
		}
	}
}

void Model::onRender()
{
	glewInit();
	shader.use();

	for (int i = 0; i < textureCount; i++) {
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}

	for (int i = 0; i < additionalTexturesCount; i++) {
		glActiveTexture(GL_TEXTURE0 + textureCount + i);
		glBindTexture(GL_TEXTURE_2D, additionalTextures[i]);
	}
	
	shader.setMatrix4x4("modelMatrix", modelMatrix.dataPointer());
	shader.setMatrix3x3("normalMatrix", normalMatrix.dataPointer());

	glBindVertexArray(VertexAttributePointer);
	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexBuffer.sizeOfData, GL_UNSIGNED_INT, 0);
}

void Model::onDeferredRender()
{
	DeferredShading::gBufferShader.use();
	for (int i = 0; i < textureCount; i++) 
	{
		textures[i].bindToShader(DeferredShading::gBufferShader, texturePropertyNames[i], i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}

	for (int i = 0; i < additionalTexturesCount; i++) 
	{
		glActiveTexture(GL_TEXTURE0 + textureCount + i);
		glBindTexture(GL_TEXTURE_2D, additionalTextures[i]);
	}

	unsigned int matrices = glGetUniformBlockIndex(DeferredShading::gBufferShader.ID, "Matrices");
	glUniformBlockBinding(DeferredShading::gBufferShader.ID, matrices, 0);

	DeferredShading::gBufferShader.setMatrix4x4("modelMatrix", modelMatrix.dataPointer());
	
	glBindVertexArray(VertexAttributePointer);
	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexBuffer.sizeOfData, GL_UNSIGNED_INT, 0);

	for (int i = 0; i < textureCount + additionalTexturesCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}

void Model::drawNoShader() const
{
	glBindVertexArray(VertexAttributePointer);
	glDrawElements(GL_TRIANGLES, indexBuffer.sizeOfData, GL_UNSIGNED_INT, 0);

}

