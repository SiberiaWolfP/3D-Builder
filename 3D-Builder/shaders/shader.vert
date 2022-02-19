#version 440 core
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;
//layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexCoord;

out vec3 Normal;
out vec3 FragPos;
//out vec4 color;
out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	//color = vColor;
	TexCoord = vTexCoord;
	gl_Position = projection * view * model * vPosition;
	FragPos = vec3(model * vPosition);
	Normal = mat3(transpose(inverse(model))) * vNormal;
}