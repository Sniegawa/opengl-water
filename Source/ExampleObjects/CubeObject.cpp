#include "CubeObject.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

CubeObject::CubeObject()
	: m_ModelMatrix(1.0f),m_Position(0.0f),m_Rotation(0.0f),m_Scale(1.0f)
{
	float vertices[] = {
		// positions          // normals           // UVs
		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		// back
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		// left
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		// right
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 // top
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 // bottom
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0,  1,  2,   0,  2,  3,  // front
		5,  4,  7,   5,  7,  6,  // back  (reversed)
		8,  9,  10,  8,  10, 11, // left
		12, 13, 14,  12, 14, 15, // right
		16, 17, 18,  16, 18, 19, // top
		20, 21, 22,  20, 22, 23, // bottom
	};

	m_vao.Bind();
	m_vbo.Bind();
	m_ebo.Bind();


	m_vbo.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);

	m_ebo.SetData(indices, sizeof(indices));

	constexpr GLsizei stride = 8 * sizeof(float);


	m_vao.linkAttrib(0, 3, GL_FLOAT, stride, (void*)0);
	m_vao.linkAttrib(1, 3, GL_FLOAT, stride, (void*)(3 * sizeof(float)));
	m_vao.linkAttrib(2, 2, GL_FLOAT, stride, (void*)(6 * sizeof(float)));

	m_vao.Unbind();
	m_vbo.Unbind();
	m_ebo.Unbind();
}

CubeObject::~CubeObject()
{
}

void CubeObject::Draw()
{
	if (!m_Texture || !m_Shader)
		return;

	m_Shader->Use();
	m_Shader->setMat4x4("ModelMatrix", m_ModelMatrix);
	m_Texture->Bind(0);
	
	m_vao.Bind();

	const int indexes = 36;

	glDrawElements(GL_TRIANGLES, indexes, GL_UNSIGNED_INT, 0);

	m_Texture->Unbind();
	m_vao.Unbind();
}

void CubeObject::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
	RecalculateModelMatrix();
}

void CubeObject::SetRotation(const glm::vec3& rot)
{
	m_Rotation = rot;
	RecalculateModelMatrix();
}

void CubeObject::Rotate(const glm::vec3& rot)
{
	m_Rotation += rot;
	RecalculateModelMatrix();
}

void CubeObject::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
	RecalculateModelMatrix();
}

void CubeObject::SetTexture(std::shared_ptr<Texture2D>& texture)
{
	m_Texture = texture;
}

void CubeObject::SetShader(std::shared_ptr<Shader>& shader)
{
	m_Shader = shader;
}

void CubeObject::RecalculateModelMatrix()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(m_Rotation)));

	m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) 
		* rotation 
		* glm::scale(glm::mat4(1.0f), m_Scale);
}
