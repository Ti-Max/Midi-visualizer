#pragma once
#include "GL/glew.h"
#include "GLFW/GLFW.h"
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

	int height;
	int width;
};

