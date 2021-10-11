//TODO: streaming files, in stead of uning one buffer. Because of the memory
//TODO: rewind
//TODO: mp3 support, converter
//TODO: better midi parser

#include "Window.h"
#include "VAO/VAO.h"
#include "VAO/Meshes.h"
#include "Shader/Shader.h"

#include "MidiFile/Midifile.h"
#include "Audio/Audio.h"
#include "Visualization/Visualization.h"
using namespace smf;

Window::Window(const std::string& title, int width, int height)
{
	createWindow(title, width, height);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void Window::createWindow(const std::string& title, int width, int height)
{
	if (!glfw.inIt())
		return;

	this->height = height;
	this->width = width;

	window = glfwCreateWindow(width, height + 1, title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPos(window, width / 2, height / 2);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowPos(window, 1920 - width, 200);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to connect GLEW" << std::endl;
	}

	//ViewPort
	int widthPX, heightPX;
	glfwGetFramebufferSize(window, &widthPX, &heightPX);
	glViewport(0, 0, widthPX, heightPX);

	//vsync
	glfwSwapInterval(1);

}
int scroll = 0;
bool isCtr = false;
Visualization render;
void Window::loop()
{
	//Audio
	Audio::init();
	Audio::addSource(new SoundSource(SourceInfo(1.0f)), "Background");
	Audio::addBuffer(new SoundBuffer("res/LoseYourself.wav"), "music");

	//Audio::addSource(new SoundSource(SourceInfo()), "click");
	//Audio::addBuffer(new SoundBuffer("res/click.wav"), "click");




	

 
	render.addMidiTracks("res/LoseYourself.mid", { {1, LINES_PATTERN, 0.0f} });

	//unsigned int tracks2[1] = {3};
	//render.addMidiTracks("res/ThemeA.mid", tracks2, 2);

	render.Start();
	Audio::Play("Background", "music");
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);// light blue
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		render.Draw();

		glfwSwapBuffers(window);
	}
}

GLFWwindow* Window::getGlfwWindow()
{
	return window;
}

Window::~Window()
{
	delete Audio::get();
	glfwDestroyWindow(window);
}
int x, y, xPos, yPos;
void setFullScreenMode(GLFWwindow* window)
{
	if (glfwGetWindowMonitor(window))
	{
		glfwSetWindowMonitor(window, nullptr, xPos, yPos, x, y, NULL);
	}
	else
	{
		//backup
		glfwGetWindowSize(window, &x, &y);
		glfwGetWindowPos(window, &xPos, &yPos);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 1000, 300, mode->width, mode->height, mode->refreshRate);
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
int lastScroll = 0;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
		setFullScreenMode(window);
	if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_RELEASE))
		isCtr = !isCtr;
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!Audio::isPaused("Background"))
			Audio::Pause("Background"), render.Pause();
		else
			Audio::Play("Background"), render.Resume();
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll += yoffset;
}
