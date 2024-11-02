#pragma once
#include <string>
#include "SmoothieMath.h"
#include <iostream>

enum ShaderType {
	VERTEX_SHADER,
	FRAGMENT_SHADER
};


enum ShaderPass
{
	PASS1 = 1, PASS2 = 2
};

using namespace SmoothieMath;
//Encapsulates shader related functions from OpenGL 
class Shader
{
	
	std::string readSourceShaderFile(std::string file);
	std::string readSignleShaderSourceFile(std::string file, ShaderType type);


	unsigned int vertexShader;
	unsigned int fragmentShader;

	
public:
	unsigned int ID;

	//Determans when object with this shader should be rendered in a render pipeline.
	ShaderPass pass;
	
	Shader(const std::string& vertexShaderSourceFile, const std::string& fragmentShaderSourceFIle);
	
	Shader(const std::string& singleShaderSourceFile);

	Shader(): vertexShader(-1), fragmentShader(-1), ID(-1){};
	//Use this shader for rendering
	void use();
	
	//Sets uniforme Bool value to be used in shader
	void setBool(const std::string& name, bool value);

	//Sets uniforme int value to be used in shader
	void setInt(const std::string& name, int value);

	//Sets uniforme float value to be used in shader
	void setFloat(const std::string& name, float value);
	
	void setMatrix4x4(const std::string& name, float* value);
	
	void setMatrix3x3(const std::string& name, float* value);

	void setVector3(const std::string& name, const Vector3& value);
};

