#pragma once
#include <iostream>
#include <map>
#include <GL/glew.h>
#include "VAO.h"
class Meshes
{
public:
	Meshes();
	void load();
	GL::VAO* operator[](const char* name);
private:
	std::map<std::string, GL::VAO*> meshes;

	GL::VAO floor, mirrorPlane, fullScreenQuad, cube, cubeNotTexture;
};

