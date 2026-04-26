#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertPath, const char* fragPath)
{
	std::string vertSrc = readFile(vertPath);
	std::string fragSrc = readFile(fragPath);

	const char* vCode = vertSrc.c_str();
	const char* fCode = fragSrc.c_str();

	unsigned int vert = compile(GL_VERTEX_SHADER, vCode, "VERTEX");
	unsigned int frag = compile(GL_FRAGMENT_SHADER, fCode, "FRAGMENT");

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vert);
	glAttachShader(m_ID, frag);
	glLinkProgram(m_ID);

	checkLinkErrors(m_ID);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Use() const
{
	glUseProgram(m_ID);
}

void Shader::setBool(const std::string& name, bool v) const
{
	glUniform1i(loc(name), v);
}

void Shader::setInt(const std::string& name, int v) const
{
	glUniform1i(loc(name), v);
}

void Shader::setFloat(const std::string& name, float v) const
{
	glUniform1f(loc(name), v);
}

void Shader::setVector2(const std::string& name, const glm::vec2& v) const
{
	glUniform2fv(loc(name), 1, glm::value_ptr(v));
}

void Shader::setVector3(const std::string& name, const glm::vec3& v) const
{
	glUniform3fv(loc(name), 1, glm::value_ptr(v));
}

void Shader::setVector4(const std::string& name, const glm::vec4& v) const
{
	glUniform4fv(loc(name), 1, glm::value_ptr(v));
}

void Shader::setMat4x4(const std::string& name, const glm::mat4& v) const
{
	glUniformMatrix4fv(loc(name), 1, GL_FALSE, glm::value_ptr(v));
}

int Shader::loc(const std::string& name) const
{
	return glGetUniformLocation(m_ID, name.c_str());
}

std::string Shader::readFile(const char* path)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::stringstream ss;
	try
	{
		file.open(path);
		ss << file.rdbuf();
	}
	catch (std::ifstream::failure& e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_READ: " << path << "\n";
	}
	return ss.str();
}

unsigned int Shader::compile(GLenum type, const char* src, const char* typestr)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::" << typestr << "::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	return shader;
}

void Shader::checkLinkErrors(unsigned int program)
{
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}
}
