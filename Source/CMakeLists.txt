cmake_minimum_required(VERSION 3.16)

project(templateApp)

add_executable(templateApp
	"main.cpp"
	"App.cpp"
	"App.h"
	"OpenGLObjects/VAO.h"
	"OpenGLObjects/VAO.cpp" 
	"OpenGLObjects/VBO.h" 
	"OpenGLObjects/VBO.cpp" 
	"OpenGLObjects/Shader.h" 
	"OpenGLObjects/Shader.cpp" "OpenGLObjects/EBO.h" "OpenGLObjects/EBO.cpp" "OpenGLObjects/Texture2D.cpp" "ExampleObjects/CubeObject.h" "ExampleObjects/CubeObject.cpp" "OpenGLObjects/Camera.h" "OpenGLObjects/Camera.cpp")


target_link_libraries(templateApp PUBLIC
	GLAD
	glfw
)

target_include_directories(templateApp PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}/
	${CMAKE_SOURCE_DIR}/libs/GLAD/include
	${CMAKE_SOURCE_DIR}/libs/glfw/include
	${CMAKE_SOURCE_DIR}/Libs/glm
)

if(MSVC)
add_custom_command(TARGET templateApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Shaders"
        "${CMAKE_CURRENT_BINARY_DIR}/Shaders"
)

add_custom_command(TARGET templateApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Textures"
        "${CMAKE_CURRENT_BINARY_DIR}/Textures"
)

endif()