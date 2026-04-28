#version 460 core

const float PI = 3.14159265359;
const float g = 9.81;
const float e = 2.71828;

layout (location = 0) in vec3 aPos;

out vec3 VertNormal;
out vec3 FragPos;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 InvModelMatrix;

uniform float u_Time;

struct Wave
{
	vec2 Direction;
	float Length;
	float Speed;
};

layout(std430,binding = 1) readonly buffer WavesBuffer
{
	Wave waves[];
};

uniform int u_WaveCount;

uniform float u_AmplitudeBase;
uniform float u_AmplitudeMult;

uniform float u_FrequencyBase;
uniform float u_FrequencyMult;

void main()
{
	vec3 p = (ModelMatrix * vec4(aPos,1.0)).xyz;
	vec3 Pos = p;
	float LastDerivativeX = 0.0;
	float LastDerivativeZ = 0.0;
	
	float DerivativeX = 0.0;
	float DerivativeZ = 0.0;

	float A = u_AmplitudeBase;
	float W = u_FrequencyBase;

	for(int i = 0; i < u_WaveCount; i++)
	{
		A = u_AmplitudeBase * pow(u_AmplitudeMult,i);
		W = u_FrequencyBase * pow(u_FrequencyMult,i);

		Wave wave = waves[i];
		float L = wave.Length;
		float phi = wave.Speed * PI / wave.Length; // Pi here represents speed
		vec2 d = normalize(wave.Direction);
		float t = u_Time;
		
		float exponent = sin(dot(d,p.xz) * W + t * phi);
		float F = A * exp(exponent-1);
		Pos.y += F;

		LastDerivativeX = W * d.x * F * cos(dot(d,p.xz) * W + t * phi);
		LastDerivativeZ = W * d.y * F * cos(dot(d,p.xz) * W + t * phi);

		DerivativeX += LastDerivativeX;
		DerivativeZ += LastDerivativeZ;
	}

	vec3 tangent = vec3(1.0,DerivativeX,0.0);
	vec3 binormal = vec3(0.0,DerivativeZ,1.0);


	vec3 normal =  normalize(cross(tangent,binormal));
	VertNormal  = normalize(transpose(mat3(InvModelMatrix)) * normal);

	FragPos = Pos;

	gl_Position = ProjectionMatrix * ViewMatrix * vec4(Pos,1.0);
}