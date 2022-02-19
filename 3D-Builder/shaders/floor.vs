#version 440 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec3 ourColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	ourColor = vColor;
	// ���ذ���΢��y�Ḻ�����ƶ���ʹ�õذ���ģ�͵��治��ȫ���ϣ��������ͻ
	vec3 pos = vec3(vPos.x, vPos.y - 0.01f, vPos.z);
    gl_Position = projection * view * vec4(pos, 1.0);
}