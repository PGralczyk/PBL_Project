#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform bool isHoovered;

uniform sampler2D texture_diffuse1;

uniform float adviseWindow;


void main()
{   
    if(isHoovered && ((adviseWindow > 4.5 || (adviseWindow > 3.5 && adviseWindow < 4) ||
    (adviseWindow > 2.5 && adviseWindow < 3) || (adviseWindow > 1.5 && adviseWindow < 2)
    || (adviseWindow > 0.5 && adviseWindow < 1))))
    {
        FragColor = texture(texture_diffuse1, TexCoords) + vec4(0.5, 0.5, 0.0, 0.0);
    }
    else if(isHoovered && adviseWindow == 0)
    {
        FragColor = texture(texture_diffuse1, TexCoords) + vec4(0.2, 0.2, 0.2, 0.0);
    }
    else
    {
        FragColor = texture(texture_diffuse1, TexCoords);
    }
}