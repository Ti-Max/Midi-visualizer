#include "SquaresPattern.h"
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern int width;
extern int height;

void SquaresPattern::SetUp(ISettings* settings)
{
	vao.addBufferData({
		{ -1.0f, -1.0f, 0.0f},
		{  1.0f, -1.0f, 0.0f},
		{  1.0f,  1.0f, 0.0f},
		{ -1.0f,  1.0f, 0.0f}
		});
	vao.addEBO({
		0, 1, 2,
		2, 3, 0
		});

	square.loadModel("res/Models/frame.obj");
	shader.load("quadShader2");
}

void SquaresPattern::Draw(const NoteInfo& note, long visualizationTime)
{
	int range[2] = { 50, 80 };// 0 - 50
	int noteCount = range[1] - (range[0]);
	float size[2] = {0.03, 0.7};

	glm::mat4 model(1.0f);
	//ratio aspect
	model = glm::scale(model, glm::vec3((float)height / (float)width, 1 ,1));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 0, 1));
	//float w = width;
	//float h = height;
	//model = glm::translate(model, glm::vec3(w/h/2, 0.5, -3));


	model = glm::scale(model, glm::vec3((note.key - range[0]) * (size[1] - size[0]) / noteCount + size[0]));

	shader.use();
	shader.setMat4("mat", model);
	shader.setVec4("color", glm::vec4(1,0.4, 0.6,1));
	square.meshes[0]->draw();
	//vao.draw();
}

SquaresPattern::~SquaresPattern()
{
}
