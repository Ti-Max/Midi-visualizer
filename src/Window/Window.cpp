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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace smf;
int height;
int width;
//void setFullScreenMode(GLFWwindow* window);

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
	glfwWindowHint(GLFW_SAMPLES, 4);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to connect GLEW" << std::endl;
	}

	//ViewPort
	int widthPX, heightPX;
	glfwGetFramebufferSize(window, &widthPX, &heightPX);
	glViewport(0, 0, widthPX, heightPX);

	//setFullScreenMode(window);
	//vsync
	//glfwSwapInterval(1);

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
bool viewportResize = false;

float exposure = 0.4;
unsigned int bloomLevel = 0;
bool filtring = false;
Visualization render;
void Window::loop()
{
	//Audio
	Audio::init();
	Audio::addSource(new SoundSource(SourceInfo(1)), "Background");
	//Audio::addBuffer(new SoundBuffer("res/Never-Gonna-Give-You-Up-3.wav"), "music");
	//Audio::addBuffer(new SoundBuffer("res/ThemeA.wav"), "music");

	//render.addMidiTracks("res/ThemeA.mid",
	//	{
	//	 /*first track*/ {3, LINES_PATTERN, new LinesSetting(glm::vec2(15, 75), glm::vec3(1, 4, 1),	LINES_TYPE_VERTICAL_DOUBLE), 0,  true},
	//	 /*second track*/{2, SQUARES_PATTERN, new SquaresSettings(), 0,  true},
	//	}, true);
	Audio::addBuffer(new SoundBuffer("res/LoseYourself.wav"), "music");

	render.addMidiTracks("res/LoseYourself.mid",
		{
		 /*first track*/ {1, LINES_PATTERN, new LinesSetting(glm::vec2(15, 75), glm::vec3(0.5, 2, 0.5),	LINES_TYPE_VERTICAL_DOUBLE), 0,  true},
		 /*second track*/{2, SQUARES_PATTERN, new SquaresSettings(), 0,  true},
		}, true);

	Meshes meshes;
	meshes.load();
	GL::VAO* screenquad = meshes["textureQuad"];
	Model bloomText("res/Models/testText.obj");
	float threshold = 0;
	Shader hdrShader("hdrShader");
	hdrShader.use();
	hdrShader.setFloat("threshold", threshold);
	Shader verticalBlurShader("verticalBlur");
	Shader HorizontalBlurShader("HorizontalBlur");
	Shader mixTextures("mixTextures");
	Shader mixAndToneMapping("mixAndToneMapping");
	//Shader screenShader("screenShader");
	Shader quadShader("quadShader");
	mixTextures.use();
	mixTextures.setInt("colorTexture", 0);
	mixTextures.setInt("HDRcolorTexture", 1);
	mixAndToneMapping.use();
	mixAndToneMapping.setInt("scene", 0);
	mixAndToneMapping.setInt("bloom", 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	FrameBuffer frameBufferColor({height, width, true, 0});
	FrameBuffer frameBufferHDRcolor({height/2, width/2, true, 0 });
	FrameBuffer frameBufferBloom({ height / 2, width / 2, true, 0 });
	for (int i = 2; i <=6; i++)
	{
		framebuffers.push_back(new FrameBuffer({ height / pow2(i) , width / pow2(i), true, 0 }));//horizontal blur
		framebuffers.push_back(new FrameBuffer({ height / pow2(i), width / pow2(i) , true, 0 }));//vertical blur
	}

	FrameBuffer frameBufferNoBloom({ height, width, true, 0 });


	render.Start();
	Audio::Play("Background", "music");
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		if (viewportResize)
		{
			for (auto b : framebuffers)
				b->Resize(height, width);
			frameBufferColor.Resize(height, width);
			frameBufferHDRcolor.Resize(height, width);
			frameBufferBloom.Resize(height, width);
			frameBufferNoBloom.Resize(height, width);
			viewportResize = false;
		}

		//Render whole scene into frameBuffer
		frameBufferColor.Bind();
		//glClearColor(0.4f, 0.1f, 0.5f, 1.0f);// light blue
		glClear(GL_COLOR_BUFFER_BIT);
		quadShader.use();
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3((float)height / (float)width, 1, 1));
		quadShader.setMat4("matrix", model);
		quadShader.setVec4("color", glm::vec4(1, 0.5, 0.125, 1));
		render.Draw(BLOOM_TRACKS);
		//bloomText.meshes[0]->draw();
		frameBufferColor.Unbind();

		//get HDR texture
		frameBufferHDRcolor.Bind();
		glClear(GL_COLOR_BUFFER_BIT);
		hdrShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameBufferColor.GetColorAttachment());
		screenquad->draw();
		frameBufferHDRcolor.Unbind();
		//-----------------------------------------------------------------
		//downsamling
		unsigned int lastColorTexture = frameBufferHDRcolor.GetColorAttachment();
		for (int i = 0; i < framebuffers.size(); i++)
		{
			//blur HDR color
			framebuffers[i]->Bind();
			glClear(GL_COLOR_BUFFER_BIT);
			if(i % 2)
				verticalBlurShader.use();
			else
				HorizontalBlurShader.use();
			glBindTexture(GL_TEXTURE_2D, lastColorTexture);
			glGenerateMipmap(GL_TEXTURE_2D);
			screenquad->draw();
			framebuffers[i]->Unbind();
			lastColorTexture = framebuffers[i]->GetColorAttachment();
		}

		// frambuffer [0]  Start  (640, 360)
		// frambuffer [1]  Finish (640, 360)
		// frambuffer [2]  Start  (320, 180)
		// frambuffer [3]  Finish (320, 180)
		// frambuffer [4]  Start  (160, 90)
		// frambuffer [5]  Finish (160, 90)
		// frambuffer [6]  Start  (80, 45)
		// frambuffer [7]  Finish (80, 45)
		// frambuffer [8]  Start  (40, 22)
		// frambuffer [9]  Finish (40, 22)
		// frambuffer [10] Start  (20, 11) B
		// frambuffer [11] Finish (20, 11)
		for (int i = framebuffers.size()-3; i >= 1; i-=2)
		{
			framebuffers[i-1]->Bind();

			glClear(GL_COLOR_BUFFER_BIT);
			mixTextures.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, framebuffers[i]->GetColorAttachment());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, lastColorTexture);

			screenquad->draw();
			framebuffers[i-1]->Unbind();

			lastColorTexture = framebuffers[i - 1]->GetColorAttachment();
		}
		frameBufferBloom.Bind();

		glClear(GL_COLOR_BUFFER_BIT);
		mixTextures.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameBufferHDRcolor.GetColorAttachment());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, lastColorTexture);

		screenquad->draw();
		frameBufferBloom.Unbind();

		lastColorTexture = frameBufferBloom.GetColorAttachment();

		//=======================
		//Render whole scene into frameBuffer
		//frameBufferNoBloom.Bind();
		//glClear(GL_COLOR_BUFFER_BIT);
		//render.Draw(NO_BLOOM_TRACKS);
		//frameBufferNoBloom.Unbind();

		//glClear(GL_COLOR_BUFFER_BIT);
		//render.Draw(NO_BLOOM_TRACKS);

		mixAndToneMapping.use();
		mixAndToneMapping.setFloat("exposure", exposure);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameBufferColor.GetColorAttachment());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, frameBufferBloom.GetColorAttachment());

		screenquad->draw();
		render.Draw(NO_BLOOM_TRACKS);

		glfwSwapBuffers(window);
	}
}

GLFWwindow* Window::getGlfwWindow()
{
	return window;
}

Window::~Window()
{
	for (auto buffer : framebuffers)
		delete buffer;

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
	viewportResize = true;
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
		exposure += 0.1;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		exposure -= 0.1;
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
	//scroll += yoffset;
}
