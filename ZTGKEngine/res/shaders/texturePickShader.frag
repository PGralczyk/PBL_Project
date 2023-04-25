#version 330 core
out vec3 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform int gObjectIndex;


void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
	if(texColor.a < 0.1)
        discard;
    
	
    FragColor = vec3(float(gObjectIndex) / 255, 0.0f, 0.0f);
}