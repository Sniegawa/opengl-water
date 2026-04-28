#pragma once

#include <array>
#include <string>
#include <memory>

#include "../OpenGLObjects/VAO.h"
#include "../OpenGLObjects/VBO.h"
#include "../OpenGLObjects/Shader.h"
class Skybox
{
public:
	Skybox(const std::array<std::string, 6>& Sides, std::shared_ptr<Shader>& shader);
	~Skybox();

	void Draw();
	void BindSkyboxTexture();
private:
	void LoadCubemap(const std::array<std::string, 6>& Sides);
private:
	std::shared_ptr<Shader> m_Shader;
	VAO m_vao;
	VBO m_vbo;
	unsigned int m_ID;
};