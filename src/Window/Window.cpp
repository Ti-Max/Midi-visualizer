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
int height;
int width;


Window::Window(const std::string& title, int width, int height)
{
	createWindow(title, width, height);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void Window::createWindow(const std::string& title, int widtht, int heightt)
{
	if (!glfw.inIt())
		return;

	height = heightt;
	width = widtht;

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
	Audio::addSource(new SoundSource(SourceInfo(0.1f)), "Background");
	//Audio::addBuffer(new SoundBuffer("res/Never-Gonna-Give-You-Up-3.wav"), "music");
	Audio::addBuffer(new SoundBuffer("res/ThemeA.wav"), "music");

	render.addMidiTracks("res/ThemeA.mid",
		{
		 /*second track*/{3, LINES_PATTERN, new LinesSetting(glm::vec2(15, 75), glm::vec3(0.5, 0, 0),	LINES_TYPE_VERTICAL_DOUBLE)},
		 /*first track*/ {2, SQUARES_PATTERN, new SquaresSettings()},
		}, true);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	unsigned int frameBuffer;
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_)
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


	render.Start();
	Audio::Play("Background", "music");
	while (!glfwWindowShouldClose(window))
	{


		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);// light blue
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
void framebuffer_size_callback(GLFWwindow* window, int widtht, int heightt)
{
	width = widtht;
	height = heightt;
	glViewport(0, 0, width, height);
}
int lastScroll = 0;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
		setFullScreenMode(window);
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
