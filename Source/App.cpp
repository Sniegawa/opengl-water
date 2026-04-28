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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

struct Wave
{
	glm::vec2 waveDirection = glm::vec2(-0.2f, 0.2f);
	float waveSteepness = 0.05f;
	float waveAmplitude = 5.0f;
	float waveLength = 250.0f;
	float _pad1 = 0.0f;
	float _pad2 = 0.0f;
	float _pad3 = 0.0f;
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
	std::shared_ptr<Shader> CubeShader = std::make_shared<Shader>("Shaders/cube.vert", "Shaders/cube.frag");
	std::shared_ptr<Texture2D> CubeTexture = std::make_shared<Texture2D>("Textures/wall.jpg");
	std::shared_ptr<Texture2D> SandTexture = std::make_shared<Texture2D>("Textures/sand.jpg");
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
	cubefloor.SetTexture(SandTexture);

	cubefloor.SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));

	cubefloor.SetScale(glm::vec3(150.0f, 1.0f, 150.0f));

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	Plane WaterPlane = Plane(100,100);
	WaterPlane.SetScale(0.25f);
	
	PerspectiveCamera camera;

	std::vector<Wave> Waves;
	Waves.resize(2);

	SSBO WaterSSBO;
	WaterSSBO.Upload<Wave>(Waves, GL_DYNAMIC_DRAW);

	double lastTime = glfwGetTime();

	bool MouseLocked = true;

	glm::vec3 LightDirection = glm::vec3(0.2f, -0.8f, 0.5f);
	glm::vec3 AmbientLight = glm::vec3(0.1f);  
	glm::vec3 DiffuseLight = glm::vec3(0.8f);
	glm::vec3 SpecularLight = glm::vec3(0.8f);
	float SpecularStrength = 0.5f;
	float Shininess = 256;
	WaterShader.Use();  



 	glm::mat4 invWaterModelMatrix = glm::inverse(WaterPlane.GetModelMatrix());
	WaterShader.setMat4x4("InvModelMatrix", invWaterModelMatrix);   

	while (!glfwWindowShouldClose(m_Window) && !glfwGetKey(m_Window,GLFW_KEY_ESCAPE))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Water Settings");
		bool Modified = false;
		bool SizeChanged = false;

		if(ImGui::Button("+"))
		{
			Waves.push_back(Wave());
			SizeChanged = true;
		}

		ImGui::SameLine();
		
		if(ImGui::Button("-"))
		{
			if (Waves.size() > 1)
			{
				Waves.pop_back();
				SizeChanged = true;
			}
		}


		for (size_t i = 0; i < Waves.size(); ++i)
		{

			std::string Title = "Wave " + std::to_string(i+1);
			ImGui::PushID(i);
			if (ImGui::TreeNode(Title.c_str()))
			{
				Wave& w = Waves[i];
				if (ImGui::SliderFloat("waveSteepness", &w.waveSteepness, 0.0f, 1.0f))
					Modified = true;
				if(ImGui::SliderFloat("waveAmplitude", &w.waveAmplitude, 1.0f, 10.0f))
					Modified = true;
				if(ImGui::SliderFloat("Length", &w.waveLength, 5.0f, 500.0f))
					Modified = true;
				if(ImGui::SliderFloat2("Wave Direction", glm::value_ptr(w.waveDirection), -1.0, 1.0))
					Modified = true;

				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		ImGui::End();

		if (Modified)
			WaterSSBO.Update<Wave>(Waves);
		else if (SizeChanged)
			WaterSSBO.Upload<Wave>(Waves);

		ImGui::Begin("Light");

		ImGui::SliderFloat3("lightDirection", glm::value_ptr(LightDirection), -1.0f, 1.0f);
		ImGui::SliderFloat("SpecularStrength", &SpecularStrength, 0.1f, 1.0f);
		ImGui::SliderFloat("Shininess", &Shininess, 2.0f, 1024.0f);

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
		
		CubeShader->Use();
		CubeShader->setMat4x4("ViewMatrix", camera.GetViewMatrix());
		CubeShader->setMat4x4("ProjectionMatrix", camera.GetProjectionMatrix());
		cube.Rotate({ 0.0f, dt*50, 0.0f });
		cube.Draw();
		cubefloor.Draw();

		

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
		WaterShader.setInt("u_WaveCount", Waves.size());
		WaterSSBO.Bind(1);


		WaterPlane.Draw();
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