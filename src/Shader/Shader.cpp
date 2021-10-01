#include "Shader.h"

Shader::Shader(const std::string& name)
{
	GLuint vertex = loadShader("res/shaders/" + name + ".vs", GL_VERTEX_SHADER);
	GLuint fragment = loadShader("res/shaders/" + name + ".fs", GL_FRAGMENT_SHADER);
	link(vertex, fragment);
}

Shader::Shader()
{
}

void Shader::load(const std::string& name)
{
	GLuint vertex = loadShader("res/shaders/" + name + ".vs", GL_VERTEX_SHADER);
	GLuint fragment = loadShader("res/shaders/" + name + ".fs", GL_FRAGMENT_SHADER);
	link(vertex, fragment);
}

GLuint Shader::loadShader(const std::string& path, GLenum type)
{
	std::ifstream fis(path);
	std::string shaderCode = { std::istreambuf_iterator<char>(fis), std::istreambuf_iterator<char>() };
	const char* c = shaderCode.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &c, NULL);
	glCompileShader(shader);


	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	return shader;
}

void Shader::link(GLuint vertex, GLuint fragment)
{
	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, fragment);
	glLinkProgram(Program);

	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	else {
		//std::cout << "Shader program was compiled" << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
void Shader::use()
{
	glUseProgram(Program);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(Program, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(Program, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(Program, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(Program, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(Program, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}