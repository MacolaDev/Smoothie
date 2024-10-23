#include "exporter.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
int packNormal(const vec3& normal) 
{
	int x_byte = int((1 + normal.x) * 127) & 0xFF;
	int y_byte = int((1 + normal.y) * 127) & 0xFF;
	int z_byte = int((1 + normal.z) * 127) & 0xFF;
	int out = (((x_byte << 0) | (z_byte << 8)) | (y_byte << 16)) | 0;
	return out;
}

std::vector<vec3> PythonListTovec3(PyObject* pyList) 
{
	std::vector<vec3> output;
	for (int i = 0; i < PyList_Size(pyList) / 3; i++)
	{
		float x = (float)PyFloat_AsDouble(PyList_GetItem(pyList, 3*i));
		float y = (float)PyFloat_AsDouble(PyList_GetItem(pyList, 3*i+1));
		float z = (float)PyFloat_AsDouble(PyList_GetItem(pyList, 3*i+2));
		output.push_back({ x, y, z });
	}
	return output;
}

std::vector<vec2> PythonListTovec2(PyObject* pyList)
{
	std::vector<vec2> output;
	for (int i = 0; i < PyList_Size(pyList) / 2; i++)
	{
		float x = (float)PyFloat_AsDouble(PyList_GetItem(pyList, 2 * i));
		float y = (float)PyFloat_AsDouble(PyList_GetItem(pyList, 2 * i + 1));
		output.push_back({ x, y });
	}
	return output;
}

PyObject* write_geometry_file(PyObject* self, PyObject* args)
{	
	//Get data from blender
	PyObject* geometryFilePath = PyObject_GetAttrString(args, "geometryFilePath");

	PyObject* numberOfvertices = PyObject_GetAttrString(args, "numberOfvertices");
	PyObject* vertexCoordinates = PyObject_GetAttrString(args, "vertexCoordinates");
	PyObject* vertexUVs = PyObject_GetAttrString(args, "vertexUVs");
	PyObject* vertexNormals = PyObject_GetAttrString(args, "vertexNormals");
	PyObject* vertexTangents = PyObject_GetAttrString(args, "vertexTangents");
	PyObject* vertexBitangents = PyObject_GetAttrString(args, "vertexBitangents");
	PyObject* vertexColors = PyObject_GetAttrString(args, "vertexColors");

	PyObject* numberOfIndices = PyObject_GetAttrString(args, "numberOfIndices");
	PyObject* indices = PyObject_GetAttrString(args, "indices");
	
	//Convert it to C++ types
	std::string file = PyUnicode_AsUTF8(geometryFilePath);
	int nOfVertices = (int)PyLong_AsLong(numberOfvertices);

	std::vector<vec3> coordinates = PythonListTovec3(vertexCoordinates);
	std::vector<vec2> UVs = PythonListTovec2(vertexUVs);
	std::vector<vec3> normals = PythonListTovec3(vertexNormals);
	std::vector<vec3> tangents = PythonListTovec3(vertexTangents);
	std::vector<vec3> bitangents = PythonListTovec3(vertexBitangents);

	int nOfIndices = (int)PyLong_AsLong(numberOfIndices);

	//Create vertex buffer
	std::vector<XYZNUVTB> buffer;
	for (int i = 0; i < nOfVertices; i++)
	{
		XYZNUVTB vertex;
		vertex.coordinate.x = coordinates[i].x;
		vertex.coordinate.y = coordinates[i].z;
		vertex.coordinate.z = coordinates[i].y;
		
		vertex.normal = packNormal(normals[i]);
		vertex.uv = UVs[i];
		vertex.tangent = packNormal(tangents[i]);
		vertex.binormal = packNormal(bitangents[i]);
		
		buffer.push_back(vertex);
	}
	
	//Create index buffer
	//TODO: make reading from py list in seperate funcion
	std::vector<unsigned int> indexes;
	for (int i = 0; i < nOfIndices; i++)
	{
		indexes.push_back(PyLong_AsLong(PyList_GetItem(indices, i)));
	}


	//Output file
	std::ofstream outFile(file, std::ios::binary);
	if (!outFile) 
	{
		std::cout << "Can't open file " << file << " for writing data!" << std::endl;
		return Py_None;
	}

	int vertexBufferOffset = 20;
	int vertexBufferType = 0; //0 for XYZNUVTB
	int vertexBufferSize = (int)buffer.size() * sizeof(XYZNUVTB);
	int indexBufferSize = (int)indexes.size() * sizeof(unsigned int);
	int indexBufferOffset = vertexBufferOffset + vertexBufferSize;
	//Vertex buffer type
	//TODO: change it to other types aswell
	outFile.write(reinterpret_cast<char*>(&vertexBufferType), sizeof(int));

	//Nubmer of vertices
	outFile.write(reinterpret_cast<char*>(&nOfVertices), sizeof(int));

	//Vertex pointer
	outFile.write(reinterpret_cast<char*>(&vertexBufferOffset), sizeof(int));

	//Number of indices
	outFile.write(reinterpret_cast<char*>(&nOfIndices), sizeof(int));

	//Index pointer
	outFile.write(reinterpret_cast<char*>(&indexBufferOffset), sizeof(int));

	//Write vertex buffer
	outFile.write(reinterpret_cast<char*>(buffer.data()), vertexBufferSize);

	//Write index buffer
	outFile.write(reinterpret_cast<char*>(indexes.data()), indexBufferSize);

	outFile.close();
	return Py_None;
}