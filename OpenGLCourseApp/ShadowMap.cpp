#include "ShadowMap.h"
using namespace SMN;

ShadowMap::ShadowMap() :FBO{ 0 },shadowMap{0}{}

bool ShadowMap::Init
(
	GLuint width, GLuint height
)
{
	this->shadowWidth = width;
	this->shadowHeight = height;

	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	float bcolour[4]{ 1.0f,1.0f,1.0f,1.0f }; 
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bcolour);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// Connects our frame buffer to our texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0); 
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBuffer Error: " << status << std::endl;
		return false;
	}
	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}	

void ShadowMap::Write()
{
	// We're able to write it
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}

void ShadowMap::Free()
{
	if (FBO) // if it is not unbined to 0
	{
		glDeleteFramebuffers(1, &FBO);
	}
	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}

ShadowMap::~ShadowMap() { Free(); }
