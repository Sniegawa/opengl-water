#version 460 core

const float PI = 3.14159265359;
const float g = 9.81;


layout (location = 0) in vec3 aPos;

out vec4 VertColor;
out vec3 VertNormal;
out vec3 FragPos;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 InvModelMatrix;

uniform float u_Time;

struct Wave
{
	vec2 waveDirection;
	float waveSteepness;
	float waveAmplitude;
	float waveLength;
	float _pad1;
	float _pad2;
	float _pad3;
};

layout(std430,binding = 1) readonly buffer WavesBuffer
{
	Wave waves[];
};

uniform int u_WaveCount;
	


vec3 GerstnerWave(float Q, float A, float L, vec2 D, vec3 P, inout vec3 tangent, inout vec3 binormal)
{
	D = normalize(D);
	float k = 2.0 * PI / L;
	float c = sqrt(g / k);	// Speed
	float f = k * (dot(D, P.xz) - c * u_Time);

	float QAk = Q * A * k;

	tangent += vec3(
		1.0 - D.x * D.x * QAk * sin(f),
		D.x * A * k * cos(f),
		-D.x * D.y * QAk * sin(f)
	);

	binormal += vec3(
		-D.x * D.y * QAk * sin(f),
		D.y * A * k * cos(f),
		1.0 - D.y * D.y * QAk * sin(f)
	);

	return vec3(
		D.x * Q * A * cos(f),
		A * sin(f),
		D.y * Q * A * cos(f)
	);

}

void main()
{
	vec3 Pos = aPos;
	vec3 p = Pos;
	vec3 tangent = vec3(0.0);
	vec3 binormal = vec3(0.0);

	for(int i = 0; i < u_WaveCount; i++)
		Pos += GerstnerWave(waves[i].waveSteepness, waves[i].waveAmplitude, waves[i].waveLength, normalize(waves[i].waveDirection), p, tangent, binormal);
	
	
	vec4 VertPos = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Pos, 1.0);

	vec3 normal =  normalize(cross(binormal,tangent));
	VertNormal  = normalize(transpose(mat3(InvModelMatrix)) * normal);

	FragPos = (ModelMatrix * vec4(Pos,1.0)).xyz;
	VertColor = vec4(0.05,0.0,0.6,1.0);

	gl_Position = VertPos;
}