#version 330 core

uniform int gObjectIndex;

out vec3 FragColor;

void main()
{
   FragColor = vec3(float(gObjectIndex) / 255, 0.0f, 0.0f);
}