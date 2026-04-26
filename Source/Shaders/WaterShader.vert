#version 330 core

const float PI = 3.14;
const float g = 9.81;


layout (location = 0) in vec3 aPos;

out vec4 VertColor;
out vec3 VertNormal;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;

uniform float u_Time;
uniform float u_WaveAmplitude;
uniform float u_WaveFrequency;
uniform float u_WaveLength;
uniform vec2  u_WaveDirection;

void main()
{
	vec3 Pos = aPos;

	float k = 2 * PI / u_WaveLength;
	float c = sqrt(g/k);
	float f = k * ((u_WaveFrequency * dot(u_WaveDirection,Pos.xz)) - c * u_Time);


	Pos.x += u_WaveDirection.x * u_WaveAmplitude * cos(f);
	Pos.y = u_WaveAmplitude * sin(f);
	Pos.z += u_WaveDirection.y * u_WaveAmplitude * cos(f);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Pos, 1.0);

	vec3 Tangent = normalize(vec3(
		1 - k * u_WaveAmplitude * sin(f),
		k * u_WaveAmplitude * cos(f),
		0
	));
	VertNormal = vec3(-Tangent.y,Tangent.x,0);

	VertColor = vec4(0.2,0.2,1.0,1.0);
}