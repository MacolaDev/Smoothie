#version 450 core

layout (location = 0) in vec3 aPos;
layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

out vec3 TexCoords;
void main()
{
	TexCoords = aPos;
	mat4 rotView = mat4(mat3(viewMatrix));
	vec4 position = projectionMatrix * rotView * vec4(aPos, 1.0);
	gl_Position = position.xyww;
}
