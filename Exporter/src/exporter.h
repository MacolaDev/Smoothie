#pragma once
#include "Python.h"

struct vec3
{
	float x, y, z;
};

struct vec2
{
	float x, y;
};

struct XYZNUVTB
{
	vec3 coordinate;
	int normal;
	vec2 uv;
	int tangent, binormal;
};

PyObject* calculate_uv_data(PyObject* self, PyObject* args);
const char calculate_uv_data_docs[] =
"Calculates UV data per vertex from per loop data.\n"
"Input object can be any object, as long as it has two list attribute named *indexes* and *data* with the same number of elements.\n"
"Returns list with per-vertex values.";

PyObject* calculate_tangent_data(PyObject* self, PyObject* args);
const char calculate_tangent_data_docs[] =
"Calculates normal/tangent/bitangent data per vertex from per loop data.\n"
"Input object can be any object, as long as it has two list attribute named *indexes* and *data* with the same number of elements.\n"
"Returns list with per-vertex values.";

PyObject* write_geometry_file(PyObject* self, PyObject* args);
const char write_geometry_file_data_docs[] =
"Takes vertex and index data and writes .geometry file";