#pragma once

#include "../OpenGLObjects/VAO.h"
#include "../OpenGLObjects/VBO.h"
#include "../OpenGLObjects/EBO.h"

#include <glm/glm.hpp>

struct PlaneVertex
{
	glm::vec3 Position = glm::vec3(0.0f);
	// glm::vec3 normal;
	// glm::vec2 UV;
};

class Plane
{
public:
	Plane(const unsigned int ResolutionX, const unsigned int ResolutionY);
	~Plane() = default;

	void Draw();

	void Resize(const unsigned int ResolutionX, const unsigned int ResolutionY);
	
	const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }
	void SetScale(float Scale);

private:
	VAO m_VAO;
	VBO m_VBO;
	EBO m_EBO;

	glm::ivec2 m_Resolution = glm::ivec2(2);

	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
};