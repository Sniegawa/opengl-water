#include "Plane.h"

#include <vector>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

void GeneratePlaneVertices(std::vector<unsigned int>& indices, std::vector<PlaneVertex>& vertices,const glm::ivec2 resolution);

Plane::Plane(const unsigned int ResolutionX, const unsigned int ResolutionY)
{
	m_Resolution = glm::ivec2(ResolutionX, ResolutionY);

	std::vector<unsigned int> indices;
	std::vector<PlaneVertex> vertices;

	GeneratePlaneVertices(indices, vertices, m_Resolution);

	m_VAO.Bind();
	m_VBO.Bind();
	m_EBO.Bind();

	m_VBO.SetData(vertices.data(), vertices.size() * sizeof(PlaneVertex), GL_STATIC_DRAW);
	m_EBO.SetData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);

	constexpr GLsizei stride = sizeof(PlaneVertex);

	m_VAO.linkAttrib(0, 3, GL_FLOAT, stride, (void*)0);

	m_VAO.Unbind();
	m_VBO.Unbind();
	m_EBO.Unbind();

}

void Plane::Resize(const unsigned int ResolutionX, const unsigned int ResolutionY)
{
	if(ResolutionX < 2 || ResolutionY < 2)
	{
		std::cout << "Plane must have resolution > 2 on both axis" << std::endl;
		return;
	}

	m_Resolution = glm::ivec2(ResolutionX, ResolutionY);

	std::vector<unsigned int> indices;
	std::vector<PlaneVertex> vertices;

	GeneratePlaneVertices(indices, vertices, m_Resolution);

	m_VAO.Bind();
	m_VBO.Bind();
	m_EBO.Bind();

	m_VBO.SetData(vertices.data(), vertices.size() * sizeof(PlaneVertex), GL_STATIC_DRAW);
	m_EBO.SetData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);

	constexpr GLsizei stride = sizeof(PlaneVertex);

	m_VAO.linkAttrib(0, 3, GL_FLOAT, stride, (void*)0);

	m_VAO.Unbind();
	m_VBO.Unbind();
	m_EBO.Unbind();

}

void Plane::Draw()
{
	const int indexes = (m_Resolution.x - 1) * (m_Resolution.y - 1) * 6;

	m_VAO.Bind();
	glDrawElements(GL_LINES, indexes, GL_UNSIGNED_INT, 0);
	m_VAO.Unbind();
}

void Plane::SetScale(float Scale)
{
	m_ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(Scale));
}

void GeneratePlaneVertices(std::vector<unsigned int>& indices, std::vector<PlaneVertex>& vertices, const glm::ivec2 resolution)
{
	const unsigned int verticesAmount = resolution.x * resolution.y;
	const unsigned int indicesSize = (resolution.x - 1) * (resolution.y - 1) * 6;
	vertices.resize(verticesAmount);
	indices.reserve(static_cast<size_t>(indicesSize));

	// Populate vertices
	for(int y = 0; y < resolution.y; ++y)
	{
		for(int x = 0; x < resolution.x; ++x)
		{
			unsigned int idx = x + (resolution.x * y);
			PlaneVertex v;

			v.Position = glm::vec3(
				static_cast<float>(x),// / (resolution.x-1),
				0.0f,
				static_cast<float>(y)// / (resolution.y-1)
			);
			vertices[idx] = v;

			// Populate inidces
			if(x < resolution.x - 1 && y < resolution.y - 1)
			{
				indices.push_back(idx);								// 0
				indices.push_back(x + 1 + resolution.x * y);		// 1
				indices.push_back(x + 1 + resolution.x * (y + 1));	// 2

				indices.push_back(idx);								// 0
				indices.push_back(x + 1 + resolution.x * (y + 1));	// 2
				indices.push_back(x + resolution.x * (y + 1));		// 3
			}
		}
	}
}