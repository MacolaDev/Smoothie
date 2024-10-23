#version 450 core
out vec4 FragColor;
in vec2 TexCoords;


float offset =  1.0 / 300.0;
vec3 Blur(sampler2D textureToBlur, vec2 coordinates)
{	
	
	vec2 offsets[9] = vec2[](
	vec2(-offset, offset), // top-left
	vec2( 0.0f, offset), // top-center
	vec2( offset, offset), // top-right
	vec2(-offset, 0.0f), // center-left
	vec2( 0.0f, 0.0f), // center-center
	vec2( offset, 0.0f), // center-right
	vec2(-offset, -offset), // bottom-left
	vec2( 0.0f, -offset), // bottom-center
	vec2( offset, -offset) // bottom-right
	);
	
	float kernel[9] = float[](
	1.0 / 16, 2.0 / 16, 1.0 / 16,
	2.0 / 16, 4.0 / 16, 2.0 / 16,
	1.0 / 16, 2.0 / 16, 1.0 / 16);
	
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(textureToBlur, coordinates.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++){
		col += sampleTex[i] * kernel[i];
	}
	return col;
}


uniform sampler2D HDRImage;
uniform sampler2D BrightSpots;
void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(HDRImage, TexCoords).rgb;
	vec3 blured = Blur(BrightSpots, TexCoords);
	
	hdrColor += blured;
	// reinhard tone mapping
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
	
	// gamma correction
	mapped = pow(mapped, vec3(1.0 / gamma));
	FragColor = vec4(mapped, 1.0);
}