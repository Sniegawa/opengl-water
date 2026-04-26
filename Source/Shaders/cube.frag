#version 330 core

in vec4 VertColor;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D texture1;


void main()
{
	FragColor = texture(texture1,UV);
}