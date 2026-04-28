#version 330 core


in vec3 VertNormal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_WaterColor;
uniform vec3 u_LightDirection;
uniform vec3 u_AmbientLight;
uniform vec3 u_DiffuseLight;
uniform vec3 u_SpecularLight;
uniform float u_Shininess;
uniform float u_SpecularStrength;
uniform vec3 u_ViewPos;

uniform samplerCube skybox;
uniform sampler2D RefractionTexture;
uniform sampler2D DuDv;

uniform float u_Time;

const vec2 screenSize = vec2(1600,900);

void main()
{
	
	vec3 Basecolor = u_WaterColor;

	vec3 lightDir = normalize(-u_LightDirection);

	vec3 ambient = u_AmbientLight;

	float diff = max(dot(VertNormal,lightDir),0.0);
	vec3 diffuse = diff * u_DiffuseLight;

	float specularStrength = u_SpecularStrength;
	vec3 viewDir = -normalize(u_ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,VertNormal);

	float spec = pow(max(dot(viewDir,reflectDir),0.0),u_Shininess);
	vec3 specular = specularStrength * spec * u_SpecularLight;


	vec3 result = (ambient + diffuse) * Basecolor +  specular;
	// Reflection
	vec3 V = -normalize(u_ViewPos - FragPos);
	vec3 N = normalize(VertNormal);

	vec3 R = reflect(-V,N);
	vec3 env = texture(skybox,R).rgb;


	float cosTheta = max(dot(N, V), 0.0);

	float F0 = 0.02;
	float fresnel = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
	vec3 color = mix(result, env, fresnel);

	FragColor = vec4(color,1.0);
}