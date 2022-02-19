#version 440
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// out float GPassVID;
// out vec3 GFragPos;
out int gl_PrimitiveID;
// ������ֵ����Ƭ��������
flat out VertexPos
{
    vec3 vertex_one;
    vec3 vertex_two;
    vec3 vertex_three;
} vertexPos;

// δ����ֵ���������������
flat out vec3 VertexIndex;
noperspective out vec3 GFragPos;

in float passVID[];
uniform mat4 ViewportMatrix; // Viewport matrix

// һ��ģ�͵ļ�����ɫ������Ҫ���ڵó�ÿ����Ƭ��������
void main()
{
	vec3 p0	= vec3(ViewportMatrix * (gl_in[0].gl_Position /
	gl_in[0].gl_Position.w));
	vec3 p1 = vec3(ViewportMatrix * (gl_in[1].gl_Position /
	gl_in[1].gl_Position.w));
	vec3 p2 = vec3(ViewportMatrix * (gl_in[2].gl_Position /
	gl_in[2].gl_Position.w));

	gl_PrimitiveID	= gl_PrimitiveIDIn;
	VertexIndex[0]	= passVID[0];
	gl_Position		= gl_in[0].gl_Position;
	GFragPos		= p0;
	vertexPos.vertex_one = p0;
	EmitVertex();

	gl_PrimitiveID	= gl_PrimitiveIDIn;
	VertexIndex[1]	= passVID[1];
	gl_Position		= gl_in[1].gl_Position;
	GFragPos		= p1;
	vertexPos.vertex_two = p1;
	EmitVertex();

	gl_PrimitiveID	= gl_PrimitiveIDIn;
	VertexIndex[2]	= passVID[2];
	gl_Position		= gl_in[2].gl_Position;
	GFragPos		= p2;
	vertexPos.vertex_three = p2;
	EmitVertex();

	EndPrimitive();
}