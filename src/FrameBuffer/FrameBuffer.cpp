#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(const FrameBufferSpec& spec)
	: spec(spec)
{
	Create();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &bufferID);
	glDeleteTextures(1, &colorTexture);
}

void FrameBuffer::Create()
{
	if (bufferID)
	{
		glDeleteFramebuffers(1, &bufferID);
		glDeleteTextures(1, &colorTexture);
	}

	glCreateFramebuffers(1, &bufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);

	GenColorTexture();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::GenColorTexture()
{
	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	


	if (spec.hdr)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, spec.width, spec.height, 0, GL_RGBA, GL_FLOAT, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spec.width, spec.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);



	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (spec.mipMapLevel)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, spec.mipMapLevel);
	}
	
	//attach
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, spec.mipMapLevel);

	glBindTexture(GL_TEXTURE_2D, 0);

}

static int pow2(int power)
{
	int n = 2;
	for (int i = 1; i < power; i++)
	{
		n *= 2;
	}
	return n;
}

void FrameBuffer::Bind()
{
	glGetIntegerv(GL_VIEWPORT, defaultViewport);
	if (spec.mipMapLevel)
		glViewport(0, 0, spec.width / pow2(spec.mipMapLevel), spec.height / pow2(spec.mipMapLevel));
	else
		glViewport(0, 0, spec.width, spec.height);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, defaultViewport[2], defaultViewport[3]);
}

void FrameBuffer::Resize(int height, int width)
{
	spec.height = height;
 	spec.width = width;
	Create();
}

unsigned int FrameBuffer::GetColorAttachment()
{
	return colorTexture;
}
