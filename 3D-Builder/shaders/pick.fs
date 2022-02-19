#version 440 core
out vec4 FragColor;

flat in vec3 VertexIndex;
in int gl_PrimitiveID;
noperspective in vec3 GFragPos;

// 未经插值的面片上三个顶点的坐标
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
    // 当点距离顶点小于设定值时为0，大于时为1
    mixVal = smoothstep(10.0f - 0.005f, 10.0f + 0.005f, d_min);
    float vertexID;
    // 当mixVal值为0时该片段顶点ID为0，当mixVal值为1时该片段顶点ID为距离更近的面片顶点ID
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
        // 给内置控件预留20个id
        FragColor = vec4(float(gObjectIndex + 20), float(gMeshIndex), float(gl_PrimitiveID), vertexID);
    }
}