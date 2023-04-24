#version 330 core

uniform int gObjectIndex;

out vec4 FragColor;

void main()
{
   FragColor = vec4(float(gObjectIndex) / 255, 0.0f, 0.0f, 1.0f);
}