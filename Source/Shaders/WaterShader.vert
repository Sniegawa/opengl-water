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

uniform float u_WaveAmplitude2;
uniform float u_WaveFrequency2;
uniform float u_WaveLength2;
uniform vec2  u_WaveDirection2;

vec3 GerstnerWave(float Amplitude, float Frequency, float Length, vec2 Direction)
{
	vec3 Pos;

	float k = 2 * PI / Length;
	float c = sqrt(g/k);
	float f = k * ((Frequency * dot(Direction,aPos.xz)) - c * u_Time);

	Pos.x = Direction.x * Amplitude * cos(f);
	Pos.y = Amplitude * sin(f);
	Pos.z = Direction.y * Amplitude * cos(f);

	return Pos;
}

void main()
{
	vec3 Pos = aPos;

	Pos += GerstnerWave(u_WaveAmplitude, u_WaveFrequency, u_WaveLength, u_WaveDirection);
	Pos += GerstnerWave(u_WaveAmplitude2, u_WaveFrequency2, u_WaveLength2, u_WaveDirection2);
	
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Pos, 1.0);

	//VertNormal = vec3(-Tangent.y,Tangent.x,0);

	VertColor = vec4(0.2,0.2,1.0,1.0);
}