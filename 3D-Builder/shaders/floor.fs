#version 440 core

in vec3 ourColor;

out vec4 FragColor;

void main()
{
    // FragColor = vec4(0.86f, 0.3f, 0.3f, 1.0f);
    FragColor = vec4(ourColor, 0.8f);
}