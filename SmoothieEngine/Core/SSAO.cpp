#include "SSAO.h"
#include <random>
#include "GL/glew.h"
#include "PostProcessing.h"

unsigned int SSAO::ssaoFBO = 0;
unsigned int SSAO::noiseTexture = 0;
unsigned int SSAO::ssaoColorBuffer = 0;

unsigned int SSAO::BluredFBO = 0;
unsigned int SSAO::BluredTexture = 0;

std::vector<Vector3> SSAO::ssaoNoise;
std::vector<Vector3> SSAO::ssaoKernel;

Shader SSAO::shaderSSAO;
Shader SSAO::blure;

void SSAO::generateSSAOTextures(int SCR_WIDTH, int SCR_HEIGHT)
{
	generateNoise();
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

	shaderSSAO = Shader("shaders/standard/SSAO.glsl");
	shaderSSAO.use();
	shaderSSAO.setInt("gPosition", 0);
	shaderSSAO.setInt("gNormal", 1);
	shaderSSAO.setInt("texNoise", 2);

	glGenFramebuffers(1, &BluredFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, BluredFBO);
	
	glGenTextures(1, &BluredTexture);
	glBindTexture(GL_TEXTURE_2D, BluredTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BluredTexture, 0);

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
			randomFloats(generator) /** 2.0 - 1.0*/, 
			randomFloats(generator) /** 2.0 - 1.0*/, 
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
		Vector3 noise(randomFloats(generator) /** 2.0 - 1.0*/, randomFloats(generator) /** 2.0 - 1.0*/, 0.0f);
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
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	
	//Add noise texture
	int uniformLocation = glGetUniformLocation(shaderSSAO.ID, "samples");
	glUniform3fv(uniformLocation, 64, ssaoKernel[0].dataPointer());
	RenderQuad();
	
	//bulre ssao texture
	glBindFramebuffer(GL_FRAMEBUFFER, BluredFBO);
	blure.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	
	RenderQuad();

}
