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

Shader::Shader(const std::string& vertexShaderSourceFile, const std::string& fragmentShaderSourceFIle)
{
	auto string = readSourceShaderFile(vertexShaderSourceFile);
	const char* vertexShaderSource = string.c_str();

	//Creates vertex shader object
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success; char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//Creates fragment shader object
	string = readSourceShaderFile(fragmentShaderSourceFIle);
	const char* fragmentShaderSource = string.c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//Attaches and links shaderProgram
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Once linked, shaders should be deleted
	glDeleteShader(vertexShader);
	SmoothieDebug(&vertexShader);
	glDeleteShader(fragmentShader);
	SmoothieDebug(&fragmentShader);
	ID = shaderProgram;

	if (fragmentShaderSourceFIle.find("pbr") != std::string::npos)
	{
		pass = ShaderPass::PASS1;
	}
	else
	{
		pass = ShaderPass::PASS2;
	}
}

Shader::Shader(const std::string& singleShaderSourceFile)
{
	auto vertexShaderSourceCode = readSignleShaderSourceFile(singleShaderSourceFile, ShaderType::VERTEX_SHADER);
	const char* vertexShaderSource = (vertexShaderSourceCode).c_str();

	//Creates vertex shader object
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success; char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//Creates fragment shader object
	std::string fragmentShaderSourceCode = readSignleShaderSourceFile(singleShaderSourceFile, ShaderType::FRAGMENT_SHADER);
	const char* fragmentShaderSource = (fragmentShaderSourceCode).c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//Attaches and links shaderProgram
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Once linked, shaders should be deleted
	glDeleteShader(vertexShader);
	SmoothieDebug(&vertexShader);
	glDeleteShader(fragmentShader);
	SmoothieDebug(&fragmentShader);
	ID = shaderProgram;

	if (singleShaderSourceFile.find("pbr") != std::string::npos)
	{
		pass = ShaderPass::PASS1;
	}
	else
	{
		pass = ShaderPass::PASS2;
	}
}

void Shader::use()
{
	glUseProgram(ID);
	SmoothieDebug(&ID);
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) 
{
	int location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, value);
	if (glGetError() != 0) std::cout << __FUNCTION__" error" << std::endl;
}

void Shader::setFloat(const std::string& name, float value) 
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix4x4(const std::string& name, float* value)
{
	
	const char* c_string = name.c_str();
	int location = glGetUniformLocation(ID, c_string);
	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void Shader::setMatrix3x3(const std::string& name, float* value)
{
	const char* c_string = name.c_str();
	int location = glGetUniformLocation(ID, c_string);
	glUniformMatrix3fv(location, 1, GL_FALSE, value);
}

void Shader::setVector3(const std::string& name, const Vector3& value)
{
	int location = glGetUniformLocation(ID, name.c_str());
	if (location != -1) {

		glUniform3fv(location, 1, value.dataPointer());
		
	}
	else {
		//std::cout << __FUNCTION__": location is -1 " << std::endl;
	}
}
