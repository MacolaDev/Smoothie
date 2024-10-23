#version 450

in vec2 TexCoord;
in vec4 FragPos;
in vec3 vertexNormals;
in mat3 TBN;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D metalnessMap;
uniform sampler2D roughnessMap;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gMRAO;

void main()
{
	vec4 MRAO = vec4(0.0f, 0.0f, 1.0f, 1.0f);

	MRAO.r = texture(metalnessMap, TexCoord).r;
	MRAO.g = texture(roughnessMap, TexCoord).r;
	
	//This version does not have alpha channel
	MRAO.b = 1.0f;

	// store the fragment position vector in the first gbuffer texture
	gPosition = FragPos;

	
	//Normal map values
	vec3 normal = texture(normalMap, TexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	gNormal = vec4(normalize(TBN * normal), 1);
	
	// and the diffuse per-fragment color
	gAlbedoSpec.rgba = texture(diffuse, TexCoord).rgba;
	gMRAO = MRAO;
	// store specular intensity in gAlbedoSpec’s alpha component
	
}