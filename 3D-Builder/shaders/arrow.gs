#version 440
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// 不经插值的面片顶点坐标

in vec4 Colors[];
noperspective out vec4 GColors;

uniform mat4 viewPort; // Viewport matrix

// 一般模型的几何着色器，主要用于得出每个面片的三个高
void main()
{
	gl_Position		= gl_in[0].gl_Position;
	GColors			= Colors[0];
	EmitVertex();

	gl_Position		= gl_in[1].gl_Position;
	GColors			= Colors[1];
	EmitVertex();

	gl_Position		= gl_in[2].gl_Position;
	GColors			= Colors[2];
	EmitVertex();

	EndPrimitive();
}