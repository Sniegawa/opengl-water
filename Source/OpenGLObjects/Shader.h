#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vertPath, const char* fragPath);

	~Shader();
	
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void Use() const;

	// Uniforms
	void setBool(const std::string& name, bool v) const;
	void setInt(const std::string& name, int v) const;
	void setFloat(const std::string& name, float v) const;
	void setVector2(const std::string& name, const glm::vec2& v) const;
	void setVector3(const std::string& name, const glm::vec3& v) const;
	void setVector4(const std::string& name, const glm::vec4& v) const;
	void setMat4x4(const std::string& name, const glm::mat4& v) const;

private:
	int loc(const std::string& name) const; // Helper for unniform locations
	static std::string readFile(const char* path);
	static unsigned int compile(GLenum type, const char* src, const char* typestr);
	static void checkLinkErrors(unsigned int program);
private:
	unsigned int m_ID;
};