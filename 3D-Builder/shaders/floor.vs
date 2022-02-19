#version 440 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec3 ourColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	ourColor = vColor;
	// 将地板略微向y轴负方向移动，使得地板与模型底面不完全贴合，避免面冲突
	vec3 pos = vec3(vPos.x, vPos.y - 0.01f, vPos.z);
    gl_Position = projection * view * vec4(pos, 1.0);
}