#pragma once
#include <vector>
#include <string>
#include "Model.h"
#include "Skybox.h"
#include <mutex>
#include <future>

class SmoothieCore;

namespace Smoothie {
	class Scene
	{
		std::vector<std::future<void>> modelFutures;
		std::string sceneFile;
	
	protected:
		std::vector<Model> gBufferModels;
		std::vector<Model> lightPassModels;
		Skybox skybox;
	public:
		friend class SmoothieCore;
		Scene();
		Scene(const std::string& sceneFile);

		//Adds a model to a scene and returns its index.
		int addModelToScene(const Model& model);

		//Returns names of all models inside a scene. Used in Smoothie Editor.
		void getAllModelNames(std::vector<std::string>& vectorToStoreNames);
	

		int getNumberOfModels();

		//Returns a pointer to a model with index ID inside loaded scene.
		Smoothie::Model* getModel(int ID);

	};
}