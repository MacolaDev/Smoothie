#pragma once
#include "SmoothieMath.h"
#include "Shader.h"
#include "PostProcessing.h"

//Bounding box that object uses.
class BoundBox: private PostProcessing
{
	static Shader boundBoxDrawingShader;

	SmoothieMath::Vector3 min;
	SmoothieMath::Vector3 max;

	SmoothieMath::Vector3 points[8];

	unsigned int boundingBoxVBO;
	unsigned int boundingBoxVAO;
	unsigned int boundingBoxIndices;

public:
	BoundBox() = default;
	BoundBox(const SmoothieMath::Vector3 min, SmoothieMath::Vector3 max);

	void drawBoundBox(SmoothieMath::Matrix4x4& modelMatrix) const;

};

