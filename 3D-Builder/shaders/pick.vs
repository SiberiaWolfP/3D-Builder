#version 440 core
layout (location = 0) in vec3 vPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
// 将顶点ID传递到片段着色器
out float passVID;

void main()
{
	// gl_VertexID是当前处理的顶点索引，当使用DrawArray绘制时由VAO中从上到下顺序增大
	// 当使用DrawElement绘制时值来自于每个索引的分量
	passVID = float(gl_VertexID);
	gl_Position = vec4(projection * view * model * vec4(vPos, 1.0));
}