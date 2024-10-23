#version 450

in vec2 TexCoord;
in vec4 FragPos;
in vec3 vertexNormals;
in mat3 TBN;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D metalnessMap;
uniform sampler2D roughnessMap;
uniform sampler2D AOMap;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gMRAO;
layout (location = 4) out vec4 gNormalValue;

float LinearizeDepth(float depth, float near, float far) {
	float ndc = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - ndc * (far - near));
}

void main()
{
	vec4 MRAO = vec4(0.0f, 0.0f, 1.0f, 1.0f);

	MRAO.r = texture(metalnessMap, TexCoord).r;
	MRAO.g = texture(roughnessMap, TexCoord).r;
	MRAO.b = texture(AOMap, TexCoord).r;

	// store the fragment position vector in the first gbuffer texture
	gPosition = FragPos;

	gNormalValue = vec4(vertexNormals, 1.0f);
	
	//Normal map values
	vec3 normal = texture(normalMap, TexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	gNormal = vec4(normalize(TBN * normal), 1);
	
	// and the diffuse per-fragment color
	gAlbedoSpec.rgba = texture(diffuse, TexCoord).rgba;
	gMRAO = MRAO;
	// store specular intensity in gAlbedoSpec’s alpha component
	
}