#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
