#version 450 core

//Vertex shader
#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (std140, binding = 0) uniform Standard
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
	vec3 viewPos;
};

out vec3 TexCoords;
void main()
{
	TexCoords = aPos;
	mat4 rotView = mat4(mat3(viewMatrix));
	vec4 position = projectionMatrix * rotView * vec4(aPos, 1.0);
	gl_Position = position.xyww;
}

#endif 

//Fragment shader
#ifdef FRAGMENT_SHADER
in vec3 TexCoords;
uniform samplerCube environmentMap;
out vec4 FragColor;

void main()
{
	vec3 image = texture(environmentMap, TexCoords).rgb;
	image = image/(image + 1.0);
	FragColor = vec4(pow(image, vec3(1/2.2f)), 1);
}
#endif 