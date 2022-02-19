#version 440
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// ������ֵ����Ƭ��������

in vec4 Colors[];
noperspective out vec4 GColors;

uniform mat4 viewPort; // Viewport matrix

// һ��ģ�͵ļ�����ɫ������Ҫ���ڵó�ÿ����Ƭ��������
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