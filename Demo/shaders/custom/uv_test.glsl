#version 450



in vec4 FragPos;

vec3 LightPosition = vec3(0.38412f,  3.1104f, -6.22991f);
vec3 LightColor = vec3(1.0f, 0.0f, 0.0f);
out vec4 FragColor;

void main()
{
    float DISTANCE = length(LightPosition - FragPos.xyz);
    float attenuation = 1.0 / (DISTANCE * DISTANCE);
    vec3 radiance = LightColor * attenuation;
	FragColor = vec4(radiance, 1);
}