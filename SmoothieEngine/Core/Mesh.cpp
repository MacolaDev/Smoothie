#include "Mesh.h"
#include <vector>
#include <fstream>
#include <string>
#include <GL/glew.h>


Mesh::Mesh(std::string filepath, bool initOpenGLFunctions) : GeometryFile(filepath)
{	
	if (isAlreadyLoaded(filepath)) 
	{
		std::cout << "Already loaded: " << filepath << std::endl;
		auto loaded = getResource(filepath);
		this->VertexBufferObject = loaded->VertexBufferObject;
		this->VertexAttributeObject = loaded->VertexAttributeObject;
		this->IndexBufferObject = loaded->IndexBufferObject;
		this->isInitilized = loaded->isInitilized;
		return;
	}

	if (initOpenGLFunctions) 
	{ 
		generateOpenGLdata(); 
		return;
	}
	isInitilized = false;
}

void Mesh::generateOpenGLdata()
{
	//Vertex buffer data
	glGenBuffers(1, &VertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, numberOfVertices * vertexTypeLenght, vertexData.data(), GL_STATIC_DRAW);


	//Vertex attribute object
	VertexAttributeObject = vertexBufferBasePtr->generateAttributeObject();

	//Index data
	glGenBuffers(1, &IndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(unsigned int), indexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	isInitilized = true;
	addResource(filepath, *this);
}

void Mesh::onRender()
{
	if (isInitilized) 
	{
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBindVertexArray(VertexAttributeObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
		glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);
	}
	else
	{
		generateOpenGLdata();
	}
}

