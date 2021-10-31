//TODO: streaming files, in stead of uning one buffer. Because of the memory
//TODO: rewind
//TODO: mp3 support, converter
//TODO: better midi parser
//h
#include "Window.h"
#include "VAO/VAO.h"
#include "VAO/Meshes.h"
#include "Shader/Shader.h"

#include "MidiFile/Midifile.h"
#include "Audio/Audio.h"
#include "Visualization/Visualization.h"
#include "FrameBuffer/FrameBuffer.h"
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
bool vBlurOn, hBlurOn = false;
int scroll = 0;
bool isCtr = false;
Visualization render;
void Window::loop()
{
	//Audio
	Audio::init();
	Audio::addSource(new SoundSource(SourceInfo(0.01f)), "Background");
	//Audio::addBuffer(new SoundBuffer("res/Never-Gonna-Give-You-Up-3.wav"), "music");
	Audio::addBuffer(new SoundBuffer("res/ThemeA.wav"), "music");

	render.addMidiTracks("res/ThemeA.mid",
		{
		 /*second track*/{3, LINES_PATTERN, new LinesSetting(glm::vec2(15, 75), glm::vec3(0.5, 0, 0),	LINES_TYPE_VERTICAL_DOUBLE)},
		 /*first track*/ {2, SQUARES_PATTERN, new SquaresSettings()},
		}, true);

	Meshes meshes;
	meshes.load();
	GL::VAO* screenquad = meshes["textureQuad"];

	Shader screenShader("screenShader");
	Shader verticalBlurShader("verticalBlur");
	Shader HorizontalBlurShader("HorizontalBlur");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FrameBuffer frameBuffer({height, width});
	FrameBuffer frameBufferBlur({height, width});


	render.Start();
	Audio::Play("Background", "music");
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		HorizontalBlurShader.use();
		HorizontalBlurShader.setBool("blurOn", hBlurOn);
		verticalBlurShader.use();
		verticalBlurShader.setBool("blurOn", vBlurOn);


		//Render whole scene into frameBuffer
		frameBuffer.Bind();
		glClearColor(0.4f, 0.1f, 0.5f, 1.0f);// light blue
		glClear(GL_COLOR_BUFFER_BIT);
		render.Draw();
		frameBuffer.Unbind();

		//Blur horizontali into frameBufferBlur
		frameBufferBlur.Bind();
		glClear(GL_COLOR_BUFFER_BIT);
		verticalBlurShader.use();
		glBindTexture(GL_TEXTURE_2D, frameBuffer.GetColorAttachment());
		screenquad->draw();
		frameBufferBlur.Unbind();

		//BlurVerticali into default frame buffer
		glClear(GL_COLOR_BUFFER_BIT);
		HorizontalBlurShader.use();
		glBindTexture(GL_TEXTURE_2D, frameBufferBlur.GetColorAttachment());
		screenquad->draw();
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
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		hBlurOn = !hBlurOn;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		vBlurOn = !vBlurOn;
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
