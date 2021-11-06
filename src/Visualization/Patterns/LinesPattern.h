#pragma once
#include "IPattern.h"
#include "VAO/VAO.h"
#include "VAO/Meshes.h"
#include "Shader/Shader.h"

#define LINES_TYPE_HORIZONTAL 0
#define LINES_TYPE_VERTICAL 1
#define LINES_TYPE_VERTICAL_DOUBLE 2

class LinesSetting : public ISettings
{
public:
	LinesSetting(glm::vec2 viewRange, glm::vec3 basicColor, int type) : viewRange(viewRange), basicColor(basicColor), type(type) {};
	~LinesSetting() override;
	//Range of notes shown on the screen
	glm::vec2 viewRange = {1, 127};
	glm::vec3 basicColor = glm::vec3(1);
	int type = LINES_TYPE_VERTICAL;
	bool bloomEnable = false;

};

class LinesPattern : public IPattern
{
public: 
	void SetUp(ISettings* settings) override;
	void Draw(const NoteInfo& note, long visualizationTime) override;
	~LinesPattern();
private:
	LinesSetting* settings;

	Meshes quadMesh;
	Shader shader;
	GL::VAO* quad;
};

