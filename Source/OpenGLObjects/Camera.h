#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class OrthographicCamera
{

};

class PerspectiveCamera
{
public:
	PerspectiveCamera();
	~PerspectiveCamera();

	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;

	void ProcessInputs(GLFWwindow* window, const float dt);

private:
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	
	const float AspectRatio = 1.6f / 0.9f;
	float Fov = 60.0f;

	glm::vec3 m_CameraPos = glm::vec3(0.0f,0.0f,3.0f);
	glm::vec3 m_Up = glm::vec3(0.0f,1.0f,0.0f);
	glm::vec3 m_Forward = glm::vec3(0.0f,0.0f,-1.0f);

	float speed = 5.0f;
};