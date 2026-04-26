#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 VertColor;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;

uniform float u_Time;

void main()
{
	vec3 Pos = aPos;
	Pos.y = sin((10*Pos.x + 10 * Pos.z) + u_Time);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Pos, 1.0);

	VertColor = vec4(0.2,0.2,1.0,1.0);
}