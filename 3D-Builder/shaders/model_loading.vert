#version 440 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoords;
layout(location = 3) in vec4 vColors;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 Colors;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 一般模型的顶点着色器
void main()
{
	TexCoords = vTexCoords;
	Normal = vNormal;
	FragPos = vec3(model * vec4(vPos, 1.0));
	Colors = vColors;
	gl_Position = vec4(projection * view * vec4(FragPos, 1.0));
}