#version 440 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale_ratio;

void main()
{
    vec4 pos = projection * view * model * vec4(vPos, 1.0f);
    vec4 nom = vec4(vNormal * scale_ratio, 1.0f);
    gl_Position = pos + nom;
}