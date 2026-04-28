#include "App.h"

#include <glad/glad.h>

#include <iostream>


#include "OpenGLObjects/VAO.h"
#include "OpenGLObjects/VBO.h"
#include "OpenGLObjects/EBO.h"
#include "OpenGLObjects/Shader.h"
#include "OpenGLObjects/Texture2D.h"

#include "OpenGLObjects/Camera.h"

#include "OpenGLObjects/SSBO.h"

#include "ExampleObjects/CubeObject.h"
#include "ExampleObjects/Plane.h"
#include "ExampleObjects/Skybox.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

struct Wave
{
	glm::vec2 waveDirection = glm::vec2(-0.2f, 0.2f);
	float waveLength = 250.0f;
	float waveSpeed = 2.5f;
};

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

	ImGui::StyleColorsDark();

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
	std::shared_ptr<Shader> CubeShader = std::make_shared<Shader>("Shaders/cube.vert", "Shaders/cube.frag");
	std::shared_ptr<Texture2D> CubeTexture = std::make_shared<Texture2D>("Textures/wall.jpg");

	Shader WaterShader("Shaders/WaterShader.vert", "Shaders/WaterShader.frag");

	std::array<std::string, 6> skyboxImages =
	{
		"Textures/skybox/right.jpg",
		"Textures/skybox/left.jpg",
		"Textures/skybox/top.jpg",
		"Textures/skybox/bottom.jpg",
		"Textures/skybox/front.jpg",
		"Textures/skybox/back.jpg"
	};

	std::shared_ptr<Shader> SkyboxShader = std::make_shared<Shader>("Shaders/Skybox.vert", "Shaders/Skybox.frag");

	 
	Skybox sb = Skybox(skyboxImages, SkyboxShader);

	//Plane GroundPlane = Plane(5, 5);
	//GroundPlane.SetScale(100);
	//GroundPlane.SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));

	CubeObject cube;

	cube.SetShader(CubeShader);
	cube.SetTexture(CubeTexture);

	cube.SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

	CubeObject cubefloor;

	cubefloor.SetShader(CubeShader);
	//cubefloor.SetTexture(CubeTexture);

	cubefloor.SetPosition(glm::vec3(0.0f, -50.0f, 0.0f));
	 
	cubefloor.SetScale(glm::vec3(150.0f, 1.0f, 150.0f));


	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	 
	Plane WaterPlane = Plane(2000,2000);
	WaterPlane.SetScale(0.05f);
	WaterPlane.SetPosition(glm::vec3(-50.0f, 0.0f, -50.0f)); 
	
	PerspectiveCamera camera;

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
	std::uniform_real_distribution<float> LengthDist(1000.0f, 1750.0f);
	std::uniform_real_distribution<float> SpeedDist(1.0f, 25.0f);

	std::vector<Wave> Waves;
	for(int i = 0; i < 64; ++i)
	{
		Wave w;
		w.waveDirection = glm::normalize(glm::vec2(dist(rng), dist(rng)));
		w.waveLength = LengthDist(rng);
		w.waveSpeed = SpeedDist(rng);
		Waves.push_back(w);
	}

	SSBO WaterSSBO;
	WaterSSBO.Upload<Wave>(Waves, GL_DYNAMIC_DRAW);

	double lastTime = glfwGetTime();

	bool MouseLocked = true;

	glm::vec3 LightDirection = glm::vec3(0.2f, 0.8f, 0.5f);
	glm::vec3 AmbientLight = glm::vec3(0.1f);  
	glm::vec3 DiffuseLight = glm::vec3(0.8f);
	glm::vec3 SpecularLight = glm::vec3(0.8f);
	float SpecularStrength = 0.5f;
	float Shininess = 256;
	WaterShader.Use();  

	float AmplitudeBase = 0.433f;
	float AmplitudeMult = 0.832f;

	float FrequencyBase = 0.418f;
	float FrequencyMult = 1.160f;

	glm::vec3 WaterColor = glm::vec3(14.0f/255, 41.0f/255, 73.0f/255);

 	glm::mat4 invWaterModelMatrix = glm::inverse(WaterPlane.GetModelMatrix());
	WaterShader.setMat4x4("InvModelMatrix", invWaterModelMatrix);   

	while (!glfwWindowShouldClose(m_Window) && !glfwGetKey(m_Window,GLFW_KEY_ESCAPE))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Waves");

		ImGui::SliderFloat("AmplitudeBase", &AmplitudeBase,0.001f,1.0f);
		ImGui::SliderFloat("AmplitudeMult", &AmplitudeMult,0.01f,0.99f);

		ImGui::SliderFloat("FrequencyBase", &FrequencyBase,0.001f,1.0f);
		ImGui::SliderFloat("FrequencyMult", &FrequencyMult,1.0f,2.0f);

		ImGui::Spacing();
		ImGui::ColorPicker3("Water Tint", glm::value_ptr(WaterColor));

		ImGui::End();

		ImGui::Begin("Light");

		ImGui::SliderFloat3("lightDirection", glm::value_ptr(LightDirection), -1.0f, 1.0f);
		ImGui::SliderFloat("SpecularStrength", &SpecularStrength, 0.1f, 1.0f);
		ImGui::SliderFloat("Shininess", &Shininess, 2.0f, 1024.0f);

		ImGui::End();

		double currentTime = glfwGetTime();
		float dt = static_cast<float>(currentTime - lastTime);
		lastTime = currentTime;

		cube.Rotate({ 0.0f, dt * 50, 0.0f });

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

		//cube.Draw();
		//cubefloor.Draw();

		// ------------- WATER ---------------  

		WaterShader.Use();
		WaterShader.setMat4x4("ViewMatrix", camera.GetViewMatrix());
		WaterShader.setMat4x4("ProjectionMatrix", camera.GetProjectionMatrix());
		WaterShader.setMat4x4("ModelMatrix", WaterPlane.GetModelMatrix());
		WaterShader.setVector3("u_ViewPos", camera.GetCameraPos());    
		WaterShader.setFloat("u_Time", currentTime);      

		WaterShader.setVector3("u_LightDirection", LightDirection);
		WaterShader.setVector3("u_AmbientLight", AmbientLight);
		WaterShader.setVector3("u_DiffuseLight", DiffuseLight);
		WaterShader.setVector3("u_SpecularLight", SpecularLight);
		WaterShader.setFloat("u_Shininess", Shininess);
		WaterShader.setFloat("u_SpecularStrength", SpecularStrength);
		WaterShader.setVector3("u_WaterColor", WaterColor);


		WaterShader.setFloat("u_AmplitudeBase", AmplitudeBase);
		WaterShader.setFloat("u_AmplitudeMult",AmplitudeMult);
		WaterShader.setFloat("u_FrequencyBase",FrequencyBase);
		WaterShader.setFloat("u_FrequencyMult",FrequencyMult);

		WaterShader.setInt("u_WaveCount", Waves.size());
		WaterSSBO.Bind(1);

		WaterShader.setInt("skybox", 0);
		sb.BindSkyboxTexture();
		 
		WaterPlane.Draw();
		  

		// ---------------- SKYBOX ------------------

		{
			SkyboxShader->Use();
			glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
			SkyboxShader->setMat4x4("ProjectionMatrix", camera.GetProjectionMatrix());
			SkyboxShader->setMat4x4("ViewMatrix", view);
		}
		glDepthFunc(GL_LEQUAL);
		sb.Draw();
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

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