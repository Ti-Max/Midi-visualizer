#pragma once
#include <GL/glew.h>
#include <iostream>
struct FrameBufferSpec
{
	int height; 
	int width;
};

class FrameBuffer
{
public:
	FrameBuffer(const FrameBufferSpec &spec);
	~FrameBuffer();
	
	void Bind();
	void Unbind();

	void Resize(int height, int width);

	unsigned int GetColorAttachment();
private:
	void Create();
	void GenColorTexture();
	unsigned int bufferID = 0;
	unsigned int colorTexture = 0;
	FrameBufferSpec spec;
};

