//TODO: streaming files, in stead of uning one buffer. Because of the memory

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

void Window::loop()
{
	//Audio
	Audio::init();
	Audio::addSource(new SoundSource(SourceInfo(0.5f)), "Background");
	Audio::addBuffer(new SoundBuffer("res/LoseYourself.wav"), "music");

	Audio::addSource(new SoundSource(SourceInfo()), "click");
	Audio::addBuffer(new SoundBuffer("res/click.wav"), "click");



	std::srand(time(0));
	
	MidiFile midifile("res/LoseYourself.mid");

	midifile.doTimeAnalysis();
	midifile.linkNotePairs();
	//midifile.makeDeltaTicks();

	int tracksCount = midifile.getTrackCount();
	MidiEventList* track_1 = &(midifile[1]);

	Visualization render;
	

	render.Start(track_1);
	Audio::Play("Background", "music");
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
		setFullScreenMode(window);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!Audio::isPaused("Background"))
			Audio::Pause("Background");
		else
			Audio::Play("Background");
	}
}
