#version 440 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColors;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 Colors;

void main()
{
	Colors = vec4(vColors, 1.0);
	gl_Position = vec4(projection * view * model * vec4(vPos, 1.0));
}