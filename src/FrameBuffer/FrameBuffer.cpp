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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spec.width, spec.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//attach
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
