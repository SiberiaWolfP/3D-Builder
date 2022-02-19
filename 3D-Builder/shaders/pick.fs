#version 440 core
out vec4 FragColor;

flat in vec3 VertexIndex;
in int gl_PrimitiveID;
noperspective in vec3 GFragPos;

// δ����ֵ����Ƭ���������������
flat in VertexPos
{
    vec3 vertex_one;
    vec3 vertex_two;
    vec3 vertex_three;
} vertexPos;


uniform int gObjectIndex;
uniform int gMeshIndex;
uniform bool debug;

void main()
{
    float distance_one = distance(GFragPos, vertexPos.vertex_one.xyz);
    float distance_two = distance(GFragPos, vertexPos.vertex_two.xyz);
    float distance_three = distance(GFragPos, vertexPos.vertex_three.xyz);

    float d_min = min(distance_one, distance_two);
    d_min = min(d_min, distance_three);
    float mixVal;
    // ������붥��С���趨ֵʱΪ0������ʱΪ1
    mixVal = smoothstep(10.0f - 0.005f, 10.0f + 0.005f, d_min);
    float vertexID;
    // ��mixValֵΪ0ʱ��Ƭ�ζ���IDΪ0����mixValֵΪ1ʱ��Ƭ�ζ���IDΪ�����������Ƭ����ID
    if (d_min == distance_one) {
        vertexID = mix(VertexIndex.x, 0.0f, mixVal);
    } else if (d_min == distance_two) {
        vertexID = mix(VertexIndex.y, 0.0f, mixVal);
    } else {
        vertexID = mix(VertexIndex.z, 0.0f, mixVal);
    }

    if (debug == true) {
        FragColor = normalize(vec4(float(gObjectIndex), float(gMeshIndex), vertexID, 1.0f));
    } else {
        // �����ÿؼ�Ԥ��20��id
        FragColor = vec4(float(gObjectIndex + 20), float(gMeshIndex), float(gl_PrimitiveID), vertexID);
    }
}