#pragma once
#include <unordered_map>
#include <string>
#include <iostream>

//Takes care of resources loading (shaders, meshes, images...) to prevent loading same "read only" resources multiple times
template <class resource> class ResourceHandler 
{
	static std::unordered_map<std::string, resource> loadedResources;

public:
	
	static resource* getResource(const std::string& resoureName);
	
	static void addResource(const std::string& resourceName, const resource& data);
	
	static bool isAlreadyLoaded(const std::string& resourceName);
};

template <class resource> std::unordered_map<std::string, resource> ResourceHandler<resource>::loadedResources;

template<class resource> 
inline resource* ResourceHandler<resource>::getResource(const std::string& resoureName)
{
	if (auto search = loadedResources.find(resoureName); search != loadedResources.end())
	{
		return &loadedResources[resoureName];
	}
	return nullptr;
}

template<class resource>
inline void ResourceHandler<resource>::addResource(const std::string& resourceName, const resource& data)
{
	const bool alreadyLoaded = isAlreadyLoaded(resourceName);
	if (alreadyLoaded)
	{
		return;
	}
	std::cout << "Resource Handler: Loading resorce file: " + resourceName << std::endl;
	loadedResources[resourceName] = data;
}

template<class resource>
inline bool ResourceHandler<resource>::isAlreadyLoaded(const std::string& resourceName)
{
	if (auto search = loadedResources.find(resourceName); search != loadedResources.end())
	{
		return true;
	}
	return false;
}
