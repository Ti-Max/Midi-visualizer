#pragma once
#include "IPattern.h"
#include "VAO/VAO.h"
#include "VAO/Meshes.h"
#include "Shader/Shader.h"

struct LinesSetting : public ISettings
{

};
class LinesPattern : public IPattern
{
public: 
	void SetUp(ISettings setting) override;
	void Draw(const NoteInfo& note, long visualizationTime) override;
private:
	Meshes quadMesh;
	Shader shader;
	GL::VAO* quad;
};

