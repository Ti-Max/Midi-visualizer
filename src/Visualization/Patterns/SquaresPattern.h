#pragma once
#include"IPattern.h"
#include "Model/Model.h"



class SquaresSettings : public ISettings
{
public:
	~SquaresSettings() override {};
};

class SquaresPattern : public IPattern
{
public:
	void SetUp(ISettings* settings) override;
	void Draw(const NoteInfo& note, long visualizationTime) override;
	~SquaresPattern();
private:
	SquaresSettings* settings;
	Model square;
	Shader shader;
	GL::VAO vao;

};

