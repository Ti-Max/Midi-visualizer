#include "LinesPattern.h"
#include "glm/ext/matrix_transform.hpp"
using namespace glm;
void LinesPattern::SetUp()
{
	quadMesh.load();
	quad = quadMesh["quad"];

	shader.load("quadShader");
	shader.use();
	shader.setVec3("color", glm::vec3(0, 1, 0));
}

void LinesPattern::Draw(const NoteInfo& note, long visualizationTime)
{
	int range[2] = { 1, 127 };// 0 - 50
	int noteCount = range[1] - (range[0]);
	const int& key = note.key;//1 - 127

	float fade = 1 - ((float)visualizationTime / CLOCKS_PER_SEC - note.noteOnTime) / note.duration;
	//float fade = 1;
	float scaling;
	float opacity = 1;
	if (fade < 0.5) {
		scaling = 4 * note.velocity / 100 * 0.25;
		opacity = fade * 2 * note.velocity / 100;
	}
	else
	{
		scaling = (4 * note.velocity / 100 * fade * fade);
	}

	mat4 mat(1.0f);
	mat = scale(mat, vec3(1.f, 1.0f / (float)noteCount, 1));// y = size of a note on display; y =
	mat = translate(mat, vec3(0, noteCount * (((float)(key - range[0]) / (float)noteCount * 2) - 1), 0));// y = -5 +5
	mat = scale(mat, vec3(1.f, scaling, 1.f));

	shader.use();
	shader.setMat4("matrix", mat);
	shader.setVec4("color", vec4(0, 0, 1, opacity));
	quad->draw();
}
