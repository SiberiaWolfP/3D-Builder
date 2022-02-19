#version 440 core
out vec4 FragColor;

noperspective in vec4 GColors;

uniform int gObjectIndex;
uniform int arrowID;
uniform bool pick;

void main()
{
    if (pick == true) {
        FragColor = vec4(float(gObjectIndex), float(arrowID), 0.0f, 0.0f);
    } else {
        FragColor = GColors;
    }
}