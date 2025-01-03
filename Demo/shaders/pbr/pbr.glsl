#version 450

//Vertex shader
#ifdef VERTEX_SHADER
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec4 aNormals;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aTangents;
layout (location = 4) in vec4 aBitangents;

layout (std140, binding = 0) uniform Standard
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
	vec3 viewPos;
    mat4 projectionViewMatrix;
};

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

vec3 unpackNormals(vec4 packedNormal)
{   
    float x = packedNormal.x/127 - 1;
    float y = packedNormal.y/127 - 1;
    float z = packedNormal.z/127 - 1;
    return vec3(x, y, z); 
}

out vec2 TexCoord;
out vec3 vertexNormals;
out vec4 FragPos;
out mat3 TBN;


void main() 
{   
    
    TexCoord = aTexCoord;
    FragPos = modelMatrix * vec4(aPos, 1);
    vertexNormals =  normalize(normalMatrix * vec3(unpackNormals(aNormals)));
	
	vec3 T = normalize(normalMatrix * unpackNormals(aTangents));
	vec3 B = normalize(normalMatrix * unpackNormals(aBitangents));
	vec3 N = normalize(normalMatrix * unpackNormals(aNormals));

    TBN = mat3(T, B, N); 
    gl_Position = projectionViewMatrix * FragPos; 
}
#endif

//Fragment shader
#ifdef FRAGMENT_SHADER
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
	// store specular intensity in gAlbedoSpecís alpha component
	
}
#endif