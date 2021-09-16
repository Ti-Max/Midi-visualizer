#include "VAO.h"

GL::VAO::VAO()
{
	//std::cout << "VAO()\n";
	glGenVertexArrays(1, &mVao);
}
GL::VAO::VAO(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	glGenVertexArrays(1, &mVao);
	addBufferData(vertices, indices);
}
void GL::VAO::genVao()
{
	glGenVertexArrays(1, &mVao);
}

void GL::VAO::draw(int count)
{
	glBindVertexArray(mVao);
	if (eboSize !=0)
		glDrawElements(GL_TRIANGLES, eboSize, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, count);
	glBindVertexArray(0);
}
void GL::VAO::drawPoints(const int count)
{
	glBindVertexArray(mVao);
	glDrawArrays(GL_POINTS, 0, count);

	glBindVertexArray(0);

}
unsigned int GL::VAO::getId()
{
	return mVao;
}
/*
void GL::VAO::draw() {
	glBindVertexArray(mVao);
		glDrawElements(GL_TRIANGLES, eboSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}*/

void GL::VAO::addBufferData(const std::vector<float>& data)
{
	glBindVertexArray(mVao);

	glEnableVertexAttribArray(buffers.size());

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(buffers.size(), 1, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	buffers.push_back(vbo);
}
void GL::VAO::addBufferData(const std::vector<glm::vec2>& data)
{
	glBindVertexArray(mVao);

	glEnableVertexAttribArray(buffers.size());

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(buffers.size(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	buffers.push_back(vbo);
}
void GL::VAO::addBufferData(const std::vector<glm::vec3>& data)
{
	glBindVertexArray(mVao);
	glEnableVertexAttribArray(buffers.size());

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(buffers.size(), 3, GL_FLOAT, GL_FALSE,0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	buffers.push_back(vbo);
}
void GL::VAO::addBufferData(const std::vector<glm::vec4>& data)
{
	glBindVertexArray(mVao);
	glEnableVertexAttribArray(buffers.size());

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec4), data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(buffers.size(), 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	buffers.push_back(vbo);
}

void GL::VAO::addBufferData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	glBindVertexArray(mVao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	addEBO(indices);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	buffers.push_back(vbo);
}

void GL::VAO::addEBO(const std::vector<unsigned int>& data)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
	eboSize = data.size();
}

GL::VAO::~VAO()
{
	//std::cout << "~VAO()\n";
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(buffers.size(), buffers.data());
	glDeleteBuffers(1, &ebo);
}