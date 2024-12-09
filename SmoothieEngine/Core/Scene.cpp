#include "Scene.h"
#include "ParseXML.h"
#include "SmoothieMath.h"
#include <sstream>
#include <GL/glew.h>

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

using ModelVectorPtr = std::vector<Smoothie::Model>*;
static std::mutex ModelMutex;
using namespace Smoothie;

static void LoadModelsAsync(ModelVectorPtr pbrModels, ModelVectorPtr otherModels, Element modelMatrixElement, Element modelFileElement)
{

	Model model = Model(modelFileElement.textContent);
	Matrix4x4 modelMatrix = getModelMatrix(modelMatrixElement);
	model.modelMatrix = modelMatrix;

	//calculate normal matrix
	Matrix4x4 normalMatrix = Matrix4x4(modelMatrix);
	normalMatrix.inverse();
	normalMatrix.transpose();
	model.normalMatrix = Matrix3x3(normalMatrix);

	std::lock_guard<std::mutex> lock(ModelMutex);

	if (model.shader.pass == ShaderPass::gBuffer)
	{
		pbrModels->push_back(model);
	}
	else
	{
		otherModels->push_back(model);
	}
}

Smoothie::Scene::Scene(const std::string& file)
{
	glewInit();
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

		Element* model_file = models[i].getChild("file");
		if (model_file == nullptr) { continue; }

		//modelFutures.push_back(std::async(std::launch::async, LoadModelsAsync, &pbrSceneModels, &OtherSceneModels, model_matrix_element, model_file));
		Model model = Model(model_file->textContent);
		
		
		Element* matrix = models[i].getChild("matrix");
		Element* position = models[i].getChild("position");
		Element* scale = models[i].getChild("scale");
		Element* rotation = models[i].getChild("rotation");
		
		if (position and rotation and scale)
		{
			model.setModelPosition(position->getVector3());
			model.setModelScale(scale->getVector3());
			model.setModelRotationEuler(rotation->getVector3());
			model.updateModelTransformMatrix();
		}
		if (matrix) 
		{
			Matrix4x4 modelMatrix = getModelMatrix(*matrix);
			model.modelMatrix = modelMatrix;
		
			//calculate normal matrix
			Matrix4x4 normalMatrix = Matrix4x4(modelMatrix);
			normalMatrix.inverse();
			normalMatrix.transpose();
			model.normalMatrix = Matrix3x3(normalMatrix);
		}

		if (model.shader.pass == ShaderPass::gBuffer) 
		{
			gBufferModels.push_back(model);
		}
		else 
		{
			lightPassModels.push_back(model);
		}
	}
}

int Smoothie::Scene::addModelToScene(const Model& model)
{
	if (model.shader.pass == ShaderPass::gBuffer)
	{
		gBufferModels.push_back(model);
	}
	else
	{
		lightPassModels.push_back(model);
	}
	return 0;
}

void Smoothie::Scene::getAllModelNames(std::vector<std::string> & vectorToStoreNames)
{
	for (int i = 0; i < gBufferModels.size(); i++) 
	{
		vectorToStoreNames.push_back(std::to_string(i) + " " + gBufferModels[i].modelFilePath);
	}
}

int Smoothie::Scene::getNumberOfModels()
{
	return static_cast<int>(gBufferModels.size());
}

Smoothie::Model* Smoothie::Scene::getModel(int ID)
{
	if (ID > gBufferModels.size()) 
	{
		std::cout << "Can't load model with index: " << ID << std::endl;
		return nullptr;
	}
	else 
	{
		return &gBufferModels[ID];
	}
}
