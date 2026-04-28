#version 330 core

in vec4 VertColor;
in vec3 VertNormal;
in vec3 FragPos;

out vec4 FragColor;


uniform vec3 u_LightDirection;
uniform vec3 u_AmbientLight;
uniform vec3 u_DiffuseLight;
uniform vec3 u_SpecularLight;
uniform float u_Shininess;
uniform float u_SpecularStrength;
uniform vec3 u_ViewPos;

void main()
{
	vec3 lightDir = normalize(-u_LightDirection);

	vec3 ambient = u_AmbientLight;

	float diff = max(dot(VertNormal,lightDir),0.0);
	vec3 diffuse = diff * u_DiffuseLight;

	float specularStrength = u_SpecularStrength;
	vec3 viewDir = normalize(u_ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,VertNormal);

	float spec = pow(max(dot(viewDir,reflectDir),0.0),u_Shininess);
	vec3 specular = specularStrength * spec * u_SpecularLight;


	vec3 result = (ambient + diffuse + specular) * VertColor.xyz;

	FragColor = vec4(result,1.0);
}