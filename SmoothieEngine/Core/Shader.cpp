#include "Shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SmoothieMath.h"
#include <GL/glew.h>
#include "SmoothieDebug.h"
using namespace SmoothieMath;


std::string Shader::readSourceShaderFile(std::string source)
{
	std::string fileContent;
	
	// Create an input file stream object
	std::ifstream file(source);

	// Check if the file is open
	if (file.is_open()) {
		// Use a stringstream to read the entire file content
		std::stringstream buffer;
		buffer << file.rdbuf();

		// Store the content into the string
		fileContent = buffer.str();

		// Close the file
		file.close();
	}
	else {
		std::cerr << "Unable to open the file: " + source << std::endl;
	}
	return fileContent;
}

std::string Shader::readSignleShaderSourceFile(std::string source, ShaderType type)
{
	std::string fileContent;

	// Create an input file stream object
	std::ifstream file(source);
	std::string shaderVersion;
	// Check if the file is open
	bool version = true;
	if (file.is_open()) {
		while (true) {
			char currentChar = file.get();
			if (currentChar == -1) break;
			if ((currentChar == '\n') and (version == true)) {
				version = false;
				if (type == ShaderType::VERTEX_SHADER) 
				{
					fileContent += "\n#define VERTEX_SHADER\n";
				}
				if (type == ShaderType::FRAGMENT_SHADER) 
				{
					fileContent += "\n#define FRAGMENT_SHADER\n";
				}
				continue;
			}
			else 
			{
				fileContent += currentChar;
			}
		}
		file.close();
	}
	else 
	{
		std::cerr << "Unable to open the file: " + source << std::endl;
	}
	return fileContent;
}

Shader::Shader(const std::string& singleShaderSourceFile, bool initilizeOpengl): ShaderFile(singleShaderSourceFile)
{
	
	this->file = singleShaderSourceFile;
	if (loadedShaders.isAlreadyLoaded(file)) 
	{ 
		Shader* original = loadedShaders.getResource(file);
		this->fragmentShaderID = original->fragmentShaderID;
		this->vertexShaderID = original->vertexShaderID;
		this->ID = original->ID;
		this->pass = original->pass;

		if (initilizeOpengl != false) generateOpenGLData();
		return;
	};

	isInitilized = false;
	
	if (initilizeOpengl == false) return;
	
	generateOpenGLData();
	
}

int Shader::VertCompInfo() const
{
	int success; char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << __FUNCTION__": can't compile shader \"" + file +"\"\n" << infoLog << std::endl;
		return 1;
	}
	return 0;
}

int Shader::FragCompInfo() const
{
	int success; char infoLog[512];
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << __FUNCTION__": can't compile shader \"" + file + "\"\n" << infoLog << std::endl;
		return 1;
	}
	return 0;
}

int Shader::GeomCompInfo() const
{
	int success; char infoLog[512];
	glGetShaderiv(geometryShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geometryShaderID, 512, NULL, infoLog);
		std::cout << __FUNCTION__": can't compile shader \"" + file + "\"\n" << infoLog << std::endl;
		return 1;
	}
	return 0;
}

int Shader::LinkInfo() const
{
	int success; char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << __FUNCTION__": can't link shader \"" + file + "\"\n" << infoLog << std::endl;
		return 1;
	}
	return 0;
}

void Shader::generateOpenGLData()
{
	const char* vertexShaderSource = (vertexShader).c_str();
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShaderID);
	if(VertCompInfo()) return;

	const char* fragmentShaderSource = (fragmentShader).c_str();
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderID);
	if(FragCompInfo()) return;
	
	if (hasGeometryShader) 
	{
		const char* geometryShaderSource = (geometryShader).c_str();
		geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShaderID, 1, &fragmentShaderSource, NULL);
		glCompileShader(geometryShaderID);
		if (GeomCompInfo()) return; 
	}

	ID = glCreateProgram();
	SmoothieDebug(&ID);
	glAttachShader(ID, vertexShaderID);
	glAttachShader(ID, fragmentShaderID);
	
	if (hasGeometryShader)
	{
		glAttachShader(ID, geometryShaderID);
	}

	glLinkProgram(ID);
	if(LinkInfo()) return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (hasGeometryShader)
	{
		glDeleteShader(geometryShaderID);
	}

	
	if (file.find("pbr") != std::string::npos)
	{
		pass = ShaderPass::gBuffer;
	}
	else
	{
		pass = ShaderPass::LightPass;
	}

	isInitilized = true;
	
	loadedShaders.addResource(file, *this);
}

void Shader::use()
{
	if (isInitilized) 
	{
		glUseProgram(ID);
	}
	else
	{
		generateOpenGLData();
	}
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const 
{
	const int location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix4x4(const std::string& name, float* value) const
{
	
	const char* c_string = name.c_str();
	const int location = glGetUniformLocation(ID, c_string);
	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void Shader::setMatrix3x3(const std::string& name, float* value) const
{
	const char* c_string = name.c_str();
	const int location = glGetUniformLocation(ID, c_string);
	glUniformMatrix3fv(location, 1, GL_FALSE, value);
}

void Shader::setVector3(const std::string& name, const Vector3& value) const
{
	const int location = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(location, 1, value.dataPointer());
		
}
