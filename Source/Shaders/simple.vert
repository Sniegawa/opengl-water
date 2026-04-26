#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec4 VertColor;
out vec2 UV;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   VertColor = vec4(aColor,1.0);
   UV = aUV;
}