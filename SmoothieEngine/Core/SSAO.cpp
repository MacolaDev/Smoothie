#include "SSAO.h"
#include <random>
#include "GL/glew.h"
#include "PostProcessing.h"

unsigned int SSAO::ssaoFBO = 0;
Texture SSAO::noiseTexture;
Texture SSAO::ssaoColorBuffer;

unsigned int SSAO::BluredFBO;
Texture SSAO::BluredTexture;

std::vector<Vector3> SSAO::ssaoNoise;
std::vector<Vector3> SSAO::ssaoKernel;

Shader SSAO::shaderSSAO;
Shader SSAO::blure;

void SSAO::generateSSAOTextures(int SCR_WIDTH, int SCR_HEIGHT)
{
	generateNoise();
	noiseTexture = Texture(&ssaoNoise[0].x, 4, 4, TextureFormat::RGB, TextureInternalFormatFloat::RGBA32);

	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	
	ssaoColorBuffer = Texture(nullptr, SCR_WIDTH, SCR_HEIGHT, RGB, RGB8);
	ssaoColorBuffer.bindToFramebuffer(ssaoFBO, 0);

	shaderSSAO = Shader("shaders/standard/SSAO.glsl");
	shaderSSAO.use();
	shaderSSAO.setInt("gPosition", 0);
	shaderSSAO.setInt("gNormal", 1);
	shaderSSAO.setInt("texNoise", 2);

	glGenFramebuffers(1, &BluredFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, BluredFBO);
	
	BluredTexture = Texture((float*)(nullptr), SCR_WIDTH, SCR_HEIGHT, TextureFormat::RGB, TextureInternalFormatFloat::RGBA32);
	BluredTexture.bindToFramebuffer(BluredFBO, 0);

	blure = Shader("shaders/blure/ssaoblure.glsl");
	blure.use();
	blure.setInt("ssao", 0);
}

static float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void SSAO::generateNoise()
{
	std::uniform_real_distribution<float> randomFloats(-1.0f, 1.0f);
	std::uniform_real_distribution<float> randomFloats01(0.0f, 1.0f);
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64; ++i)
	{
		Vector3 sample( 
			randomFloats(generator), 
			randomFloats(generator), 
			randomFloats01(generator)
		);
		sample = SmoothieMath::normalize(sample);
		sample = sample * randomFloats01(generator);

		float scale = (float)i / 64.0f;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample = sample * scale;

		ssaoKernel.push_back(sample);
	}

	for (unsigned int i = 0; i < 16; i++)
	{
		Vector3 noise(randomFloats(generator), randomFloats(generator), 0.0f);
		noise = SmoothieMath::normalize(noise);
		ssaoNoise.push_back(noise);
	}
}

void SSAO::onRender()
{

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClearColor(0.0, 0.0, 0.0, 1.0); // black so it won’t leak in g-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderSSAO.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DeferredShading::gPosition);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, DeferredShading::gNormal);
	
	noiseTexture.bind(2);
	
	//Add noise texture
	int uniformLocation = glGetUniformLocation(shaderSSAO.ID, "samples");
	glUniform3fv(uniformLocation, 64, ssaoKernel[0].dataPointer());
	RenderQuad();
	
	//bulre ssao texture
	glBindFramebuffer(GL_FRAMEBUFFER, BluredFBO);
	blure.use();

	ssaoColorBuffer.bind(0);
	
	RenderQuad();

}
