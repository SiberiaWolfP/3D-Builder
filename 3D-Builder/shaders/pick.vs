#version 440 core
layout (location = 0) in vec3 vPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
// ������ID���ݵ�Ƭ����ɫ��
out float passVID;

void main()
{
	// gl_VertexID�ǵ�ǰ����Ķ�����������ʹ��DrawArray����ʱ��VAO�д��ϵ���˳������
	// ��ʹ��DrawElement����ʱֵ������ÿ�������ķ���
	passVID = float(gl_VertexID);
	gl_Position = vec4(projection * view * model * vec4(vPos, 1.0));
}