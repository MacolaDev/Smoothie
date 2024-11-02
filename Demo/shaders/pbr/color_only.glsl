#version 450

#ifdef VERTEX_SHADER
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec4 aNormals;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aTangents;
layout (location = 4) in vec4 aBitangents;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
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


void main() 
{   
    
    TexCoord = aTexCoord;
    FragPos = modelMatrix * vec4(aPos, 1);
    //mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertexNormals =  normalize(-normalMatrix * vec3(unpackNormals(aNormals)));
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0); 
}
#endif


//Fragment shader
#ifdef FRAGMENT_SHADER
in vec2 TexCoord;
in vec4 FragPos;
in vec3 vertexNormals;

uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gMRAO;

void main()
{
	gMRAO = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	gAlbedoSpec = vec4(color, 1);
	gPosition = vec4(FragPos.xyz, 1);
	gNormal = vec4(vertexNormals, 1);
}
#endif