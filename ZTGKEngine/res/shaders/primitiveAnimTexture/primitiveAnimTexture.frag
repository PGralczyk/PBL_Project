#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float texOffset;
vec2 newTexCoords = vec2(TexCoords.x + texOffset, TexCoords.y);

void main()
{    
    FragColor = texture(texture_diffuse1, newTexCoords);
}