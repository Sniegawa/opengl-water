#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "../OpenGLObjects/VAO.h"
#include "../OpenGLObjects/VBO.h"
#include "../OpenGLObjects/EBO.h"

#include "../OpenGLObjects/Shader.h"
#include "../OpenGLObjects/Texture2D.h"

class CubeObject // Note : this should pobably use some kind of instancing
{
public:
	CubeObject();
	
	~CubeObject();

	void Draw();

	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::vec3& rot);
	void Rotate(const glm::vec3& rot);
	void SetScale(const glm::vec3& scale);
	void SetTexture(std::shared_ptr<Texture2D>& texture);
	void SetShader(std::shared_ptr<Shader>& shader);
private:
	void RecalculateModelMatrix();
private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Texture2D> m_Texture;

	VAO m_vao;
	VBO m_vbo;
	EBO m_ebo;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_ModelMatrix;
};