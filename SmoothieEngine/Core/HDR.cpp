#include "HDR.h"
#include "GL/glew.h"
#include "SmoothieDebug.h"
#include "PostProcessing.h"
HDR::HDR(int SCR_WIDTH, int SCR_HEIGHT)
{
	glewInit();
	glGenFramebuffers(1, &FramebufferID);
	SmoothieDebug(&FramebufferID);
	
	glGenTextures(1, &TextureID);
	SmoothieDebug(&TextureID);

	glGenTextures(1, &BloomTextureID);
	SmoothieDebug(&BloomTextureID);

	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, BloomTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glGenRenderbuffers(1, &RenderbufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderbufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, BloomTextureID, 0);
	
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RenderbufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//shader = Shader("shaders/HDRVertex.glsl", "shaders/HDRFragment.glsl");
	shader.use();
	shader.setInt("HDRImage", 0);
	shader.setInt("BrightSpots", 1);


	//pingPongShader = Shader("shaders/pingPongVertex.glsl", "shaders/pingPongFragment.glsl");
	pingPongShader.use();
	pingPongShader.setInt("HDRImage", 0);

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, PingPongBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, PingPongBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PingPongBuffers[i], 0);
	}
}
void HDR::bindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void HDR::unbindFrameBuffer()
{
	/*bool horizontal = true, first_iteration = true;
	int amount = 10;
	pingPongShader.use();
	
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		pingPongShader.setInt("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? PingPongBuffers[1] : PingPongBuffers[!horizontal]);
		pingPongShader.setInt("image", 0);
		glBindVertexArray(quatVertexAttribue);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}*/



	shader.use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, BloomTextureID);

	RenderQuad();
}
