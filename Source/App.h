#pragma once

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include <glm/glm.hpp>

class App
{
public:
	App();
	~App();

	void Run();

private:
	GLFWwindow* m_Window;
	glm::vec2 m_WindowSize = glm::vec2(0.0f);
};