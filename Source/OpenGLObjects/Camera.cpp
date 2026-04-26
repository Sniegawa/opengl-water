#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

PerspectiveCamera::PerspectiveCamera()
{
	m_ProjectionMatrix = glm::perspective(
		glm::radians(Fov),
		AspectRatio,
		0.1f, 1000.0f
	);
	m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_Forward, m_Up);
}

PerspectiveCamera::~PerspectiveCamera() {}

const glm::mat4& PerspectiveCamera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

const glm::mat4& PerspectiveCamera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void PerspectiveCamera::ProcessInputs(GLFWwindow* window,const float dt)
{
	static float s_Yaw = -90.0f; // facing -Z by default
	static float s_Pitch = 0.0f;
	static float s_LastX = 0.0f;
	static float s_LastY = 0.0f;
	static bool  s_FirstMouse = true;
	static float s_Sensitivity = 0.1f;

	glm::vec3 right = glm::normalize(glm::cross(m_Forward, m_Up));

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_CameraPos += m_Forward * speed * dt;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_CameraPos -= m_Forward * speed * dt;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_CameraPos -= right * speed * dt;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_CameraPos += right * speed * dt;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		m_CameraPos += m_Up * speed * dt;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		m_CameraPos -= m_Up * speed * dt;


	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (s_FirstMouse)
	{
		s_LastX = (float)mouseX;
		s_LastY = (float)mouseY;
		s_FirstMouse = false;
	}

	float deltaX = ((float)mouseX - s_LastX) * s_Sensitivity;
	float deltaY = (s_LastY - (float)mouseY) * s_Sensitivity; // inverted Y
	s_LastX = (float)mouseX;
	s_LastY = (float)mouseY;

	s_Yaw += deltaX;
	s_Pitch = std::clamp(s_Pitch + deltaY, -89.0f, 89.0f); // prevent gimbal flip

	m_Forward.x = cos(glm::radians(s_Yaw)) * cos(glm::radians(s_Pitch));
	m_Forward.y = sin(glm::radians(s_Pitch));
	m_Forward.z = sin(glm::radians(s_Yaw)) * cos(glm::radians(s_Pitch));
	m_Forward = glm::normalize(m_Forward);


	m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_Forward, m_Up);
}