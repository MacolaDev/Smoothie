#include "BoundBox.h"
#include "GL/glew.h"
#include "VertexBuffer.h"

Shader BoundBox::boundBoxDrawingShader;

const unsigned int indices[] =
{
	0, 1, 
	2, 3,
	4, 5,
	6, 7,

	0, 2,
	1, 3,

	4, 6,
	5, 7,

	0, 4,
	1, 5,

	7, 3,
	6, 2,
		
};


BoundBox::BoundBox(const SmoothieMath::Vector3 min, SmoothieMath::Vector3 max)
{
	PostProcessing::initPostProcessing();
	this->min = min;
	this->max = max;

	points[0] = { min.x, min.y, min.z };
	points[1] = { min.x, max.y, min.z };
	points[2] = { min.x, min.y, max.z };
	points[3] = { min.x, max.y, max.z };

	points[4] = { max.x, min.y, min.z };
	points[5] = { max.x, max.y, min.z };
	points[6] = { max.x, min.y, max.z };
	points[7] = { max.x, max.y, max.z };

	boundBoxDrawingShader = Shader("shaders/standard/BoundBoxDraw.glsl");

	glGenBuffers(1, &boundingBoxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, boundingBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(SmoothieMath::Vector3), points[0].dataPointer(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &boundingBoxVAO);
	glBindVertexArray(boundingBoxVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &boundingBoxIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundingBoxIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


}


void BoundBox::drawBoundBox(SmoothieMath::Matrix4x4& modelMatrix) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	boundBoxDrawingShader.use();
	boundBoxDrawingShader.setVector3("minPoint", min);
	boundBoxDrawingShader.setVector3("maxPoint", max);
	boundBoxDrawingShader.setMatrix4x4("modelMatrix", modelMatrix.dataPointer());

	glBindBuffer(GL_ARRAY_BUFFER, boundingBoxVBO);
	glBindVertexArray(boundingBoxVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundingBoxIndices);
	glDrawElements(GL_LINES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	
}
