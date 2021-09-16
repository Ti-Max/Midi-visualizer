#pragma once
#include<iostream>
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

namespace GL 
{
	class VAO
	{
	public:
		VAO();
		VAO(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		void genVao();
		~VAO();
		//В этих случаях мы создаем отдельный буффер для для каждого атрибута ###layout (location = 0) in vec3 pos###
		void addBufferData(const std::vector<float>& data);
		void addBufferData(const std::vector<glm::vec2>& data);
		void addBufferData(const std::vector<glm::vec3>& data);
		void addBufferData(const std::vector<glm::vec4>& data);
		//erfjhuds
		void addBufferData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		void addEBO(const std::vector<unsigned int>& data);

		//Using GL_TRIANGLE
		void draw(const int count = 0);
		//Points
		void drawPoints(const int count);
		//Only with EBO
		//void draw();

		unsigned int getId();
	private:
		std::vector<unsigned int> buffers;
		unsigned int mVao;
		unsigned int ebo = 0;
		unsigned int eboSize = 0;
	};
}