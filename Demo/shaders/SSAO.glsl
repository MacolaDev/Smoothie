#version 450

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 projection;

float LinearizeDepth(float depth, float near, float far) {
	float ndc = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - ndc * (far - near));
}

layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

// tile noise texture over screen, based on screen dimensions / noise size
const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0); 

float radius = 1;
float bias = 0.025;
int kernelSize = 64;
void main()
{
	vec3 FragPos = texture(gPosition, TexCoords).xyz * 2 - 1;
	vec3 normal = texture(gNormal, TexCoords).rgb * 2 - 1;
	normal = normalize(normal);
	vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz * 2 - 1;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal)) * 2 - 1;
	vec3 bitangent = cross(normal, tangent) * 2 - 1;
	mat3 TBN = mat3(tangent, bitangent, normal);

	
	float occlusion = 0.0f;
	for(int i = 0; i < kernelSize; ++i)
	{	
		// get sample position
		vec3 SAMPLE = TBN * samples[i]; // from tangent to view-space
		SAMPLE = FragPos + SAMPLE * radius;

		vec4 offset = vec4(SAMPLE, 1.0f);
		offset = projectionMatrix * offset; // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
		float sampleDepth = texture(gPosition, offset.xy).r;
		
		//float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth.z));
		
		occlusion += (sampleDepth >= SAMPLE.z + bias) ? 1.0 : 0.0; //* rangeCheck;
	}

	occlusion = 1 - (occlusion / kernelSize);
	//float depth = LinearizeDepth(texture(gPosition, TexCoords).w, 0.1f, 100.0f) / 100.0f;
	vec3 testSAMPLE = FragPos + (TBN * samples[0]) * radius;
	vec4 testOFFSET = vec4(testSAMPLE, 1);
	testOFFSET = projectionMatrix * testOFFSET;
	testOFFSET.xy /= testOFFSET.w;
	testOFFSET = testOFFSET*0.5 + 0.5;
	FragColor = vec4(vec3(occlusion), 1);

}