#version 330 core

uniform int gObjectIndex;

out vec4 FragColor;

void main()
{
    int decimalParts = (gObjectIndex % 255) / 255;
    FragColor = vec4(float(gObjectIndex % 255) / 255, float(gObjectIndex / 255), 0.0f, 1.0f);
}