#version 450 core

in vec3 TexCoords;
uniform samplerCube environmentMap;
out vec4 FragColor;

void main()
{
	vec3 image = texture(environmentMap, TexCoords).rgb;
	image = image/(image + 1.0);
	FragColor = vec4(pow(image, vec3(1/2.2f)), 1);
}