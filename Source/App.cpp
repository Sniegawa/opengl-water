#include "App.h"

#include <glad/glad.h>

#include <iostream>


#include "OpenGLObjects/VAO.h"
#include "OpenGLObjects/VBO.h"
#include "OpenGLObjects/EBO.h"
#include "OpenGLObjects/Shader.h"
#include "OpenGLObjects/Texture2D.h"

#include "OpenGLObjects/Camera.h"

#include "ExampleObjects/CubeObject.h"

#include "ExampleObjects/Plane.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

App::App()
{
	m_WindowSize = glm::vec2(1600, 900);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	m_Window = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, "opengl-water", NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return; // Make an exception or sth idc
	}

	glfwMakeContextCurrent(m_Window);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return; // Make an exception or sth idc
	}

	glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);

	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark(); // or Light(), Classic()

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true); // true = install GLFW callbacks
	ImGui_ImplOpenGL3_Init("#version 330");
	
}

App::~App()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void App::Run()
{
	float vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {  
		0, 1, 3,   
		1, 2, 3   
	};
	

	std::shared_ptr<Shader> CubeShader = std::make_shared<Shader>("Shaders/cube.vert", "Shaders/cube.frag");
	std::shared_ptr<Texture2D> CubeTexture = std::make_shared<Texture2D>("Textures/wall.jpg");
	Shader WaterShader("Shaders/WaterShader.vert", "Shaders/WaterShader.frag");

	CubeObject cube;

	cube.SetShader(CubeShader);
	cube.SetTexture(CubeTexture);

	cube.SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

	CubeObject cubefloor;

	cubefloor.SetShader(CubeShader);
	cubefloor.SetTexture(CubeTexture);

	cubefloor.SetPosition(glm::vec3(0.0f, -2.0f, -5.0f));

	cubefloor.SetScale(glm::vec3(15.0f, 1.0f, 15.0f));

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	Plane WaterPlane = Plane(100,100);
	WaterPlane.SetScale(0.125f);
	

	PerspectiveCamera camera;

	float waveAmplitude = 2.5f;
	float waveFrequency = 5.0f;
	float waveLength = 250.0f;
	glm::vec2 waveDirection = glm::vec2(0.f);

	float waveAmplitude2 = 2.5f;
	float waveFrequency2 = 5.0f;
	float waveLength2 = 250.0f;
	glm::vec2 waveDirection2 = glm::vec2(0.0f);

	double lastTime = glfwGetTime();

	bool MouseLocked = true;

	while (!glfwWindowShouldClose(m_Window) && !glfwGetKey(m_Window,GLFW_KEY_ESCAPE))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Water Settings");
		ImGui::SliderFloat("Amplitude", &waveAmplitude, 0.0f, 10.0f);
		ImGui::SliderFloat("Frequency", &waveFrequency, 0.0f, 25.0f);
		ImGui::SliderFloat("Length", &waveLength, 5.0f, 500.0f);
		ImGui::SliderFloat2("Wave Direction", glm::value_ptr(waveDirection), -1.0, 1.0);

		ImGui::SliderFloat("Amplitude2", &waveAmplitude2, 0.0f, 10.0f);
		ImGui::SliderFloat("Frequency2", &waveFrequency2, 0.0f, 25.0f);
		ImGui::SliderFloat("Length2", &waveLength2, 5.0f, 500.0f);
		ImGui::SliderFloat2("Wave Direction2", glm::value_ptr(waveDirection2), -1.0, 1.0);
		ImGui::End();

		double currentTime = glfwGetTime();
		float dt = static_cast<float>(currentTime - lastTime);
		lastTime = currentTime;

		if(MouseLocked)
			camera.ProcessInputs(m_Window,dt);

		if (glfwGetKey(m_Window, GLFW_KEY_1))
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			MouseLocked = false;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_2))
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			MouseLocked = true;
		}

		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//CubeShader->Use();
		//CubeShader->setMat4x4("ViewMatrix", camera.GetViewMatrix());
		//CubeShader->setMat4x4("ProjectionMatrix", camera.GetProjectionMatrix());
		//cube.Rotate({ 0.0f, dt*50, 0.0f });
		//cube.Draw();
		//cubefloor.Draw();

		WaterShader.Use();
		WaterShader.setMat4x4("ViewMatrix", camera.GetViewMatrix());
		WaterShader.setMat4x4("ProjectionMatrix", camera.GetProjectionMatrix());
		WaterShader.setMat4x4("ModelMatrix", WaterPlane.GetModelMatrix());
		WaterShader.setFloat("u_Time", currentTime);
		WaterShader.setFloat("u_WaveAmplitude", waveAmplitude);
		WaterShader.setFloat("u_WaveFrequency", waveFrequency);
		WaterShader.setFloat("u_WaveLength", waveLength);
		WaterShader.setVector2("u_WaveDirection", waveDirection);

		WaterShader.setFloat("u_WaveAmplitude2", waveAmplitude2);
		WaterShader.setFloat("u_WaveFrequency2", waveFrequency2);
		WaterShader.setFloat("u_WaveLength2", waveLength2);
		WaterShader.setVector2("u_WaveDirection2", waveDirection2);

		WaterPlane.Draw();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}