#version 440 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vColors;

out vec3 Colors;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 一般模型的顶点着色器
void main()
{
	Colors = vColors;
	gl_Position = vec4(projection * view * model * vec4(vPos, 1.0));
}