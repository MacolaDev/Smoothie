#pragma once
#include <vector>
#include <string>
#include "Model.h"
#include "Skybox.h"

class SmoothieCore;

namespace Smoothie {
	class Scene
	{
		std::vector<Model> pbrSceneModels;
		std::vector<Model> OtherSceneModels;
		std::string sceneFile;
		Skybox skybox;
	public:
		friend class SmoothieCore;
		Scene();
		Scene(const std::string& sceneFile);

		//Adds a model to a scene and returns its index.
		int addModelToScene(const Model& model);
	};
}