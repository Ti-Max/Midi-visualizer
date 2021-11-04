#pragma once
#include "GL/glew.h"
#include "GLFW/GLFW.h"
#include "FrameBuffer/FrameBuffer.h"
#include <vector>


class Window
{
public:
	Window(const std::string& title, int width, int height);
	Window()
	{
	}
	~Window();
	void createWindow(const std::string& title, int width, int height);
	void loop();
	GLFWwindow* getGlfwWindow();
private:
	GLFW glfw;
	GLFWwindow* window;
	std::vector<FrameBuffer*> framebuffers;

};

