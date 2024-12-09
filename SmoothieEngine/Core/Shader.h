#pragma once
#include <string>
#include <iostream>
#include "SmoothieMath.h"
#include "ResourceHandler.h"
#include "ShaderFile.h"

enum ShaderType 
{
	VERTEX_SHADER,
	FRAGMENT_SHADER, 
	GEOMETRY_SHADER
};


enum ShaderPass
{
	gBuffer = 1, LightPass = 2
};

using namespace SmoothieMath;

//Encapsulates shader related functions from OpenGL 
class Shader: private ShaderFile
{
	std::string readSourceShaderFile(std::string file);
	std::string readSignleShaderSourceFile(std::string file, ShaderType type);
	
	unsigned int vertexShaderID;
	unsigned int fragmentShaderID;
	unsigned int geometryShaderID;

	ResourceHandler<Shader> loadedShaders;

	bool isInitilized;
	

	int VertCompInfo() const;
	int FragCompInfo() const;
	int GeomCompInfo() const;
	int LinkInfo() const;


public:
	unsigned int ID;
	
	//If initilizeOpengl is set tu false, this must be called before attempting to use shader.
	//This is mainly here to allow multithreading with OpenGL.
	void generateOpenGLData();

	//Determans when object with this shader should be rendered in a render pipeline.
	ShaderPass pass;
	
	Shader(const std::string& singleShaderSourceFile, bool initilizeOpengl = true);

	Shader() = default;

	//Use this shader for rendering
	void use();
	
	//Sets uniforme Bool value to be used in shader
	void setBool(const std::string& name, bool value) const;

	//Sets uniforme int value to be used in shader
	void setInt(const std::string& name, int value) const;

	//Sets uniforme float value to be used in shader
	void setFloat(const std::string& name, float value) const;
	
	void setMatrix4x4(const std::string& name, float* value) const;
	
	void setMatrix3x3(const std::string& name, float* value) const;

	void setVector3(const std::string& name, const Vector3& value) const;
};

