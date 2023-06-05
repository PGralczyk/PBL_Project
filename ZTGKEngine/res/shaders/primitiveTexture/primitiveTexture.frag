#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform bool isHoovered;

uniform sampler2D texture_diffuse1;


void main()
{    
    if(isHoovered)
    {
        FragColor = texture(texture_diffuse1, TexCoords) + vec4(0.1, 0.1, 0.1, 0.0);
    }
    else
    {
        FragColor = texture(texture_diffuse1, TexCoords);
    }
}