#include "Scene.h"
#include "ParseXML.h"
#include "SmoothieMath.h"
#include <sstream>
Vector4 getVector4FromString(const std::string& string)
{
	std::istringstream data(string);
	float x, y, z, w;
	data >> x >> y >> z >> w;
	return Vector4(x, y, z, w);
}

Matrix4x4 getModelMatrix(const Element& element)
{
	std::string row0 = element.children[0].textContent;
	std::string row1 = element.children[1].textContent;
	std::string row2 = element.children[2].textContent;
	
	Vector4 row0vector = getVector4FromString(row0);
	Vector4 row1vector = getVector4FromString(row1);
	Vector4 row2vector = getVector4FromString(row2);
	const Vector4 row3 = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4x4(row0vector, row1vector, row2vector, row3);
}

Smoothie::Scene::Scene()
{
}

Smoothie::Scene::Scene(const std::string& file)
{

	sceneFile = file;
	//Parse xml data
	ParseXML data = ParseXML(file);
	
	//Get skybox data
	Element skybox_image = data.getElement("skybox");
	skybox = Skybox(skybox_image.textContent);
	skybox.calculateIrradiance();
	skybox.calculatePrefilteredMap();

	//Get models from scene file
	std::vector<Element> models = data.getElement("models").children;
	for (int i = 0; i < models.size(); i++)
	{
		Element model_matrix_element = models[i].children[0];
		Element model_file = models[i].children[1];
		
		Model model = Model(model_file.textContent);
		Matrix4x4 modelMatrix = getModelMatrix(model_matrix_element);
		model.modelMatrix = modelMatrix;
		
		//calculate normal matrix
		Matrix4x4 normalMatrix = Matrix4x4(modelMatrix);
		normalMatrix.inverse();
		normalMatrix.transpose();
		model.normalMatrix = Matrix3x3(normalMatrix);

		if (model.shader.pass == ShaderPass::PASS1) 
		{
			pbrSceneModels.push_back(model);
		}
		else 
		{
			OtherSceneModels.push_back(model);
		}
	}
}

int Smoothie::Scene::addModelToScene(const Model& model)
{
	if (model.shader.pass == ShaderPass::PASS1)
	{
		pbrSceneModels.push_back(model);
	}
	else
	{
		OtherSceneModels.push_back(model);
	}
	return 0;
}
