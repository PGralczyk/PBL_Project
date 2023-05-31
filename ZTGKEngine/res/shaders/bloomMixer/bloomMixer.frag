#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D highlightParts;
uniform float strength;


void main()
{             
    vec3 originalColor = texture(scene, TexCoords).rgb;     
    vec3 highlightColor = texture(highlightParts, TexCoords).rgb;
    
    FragColor = vec4(originalColor + highlightColor * strength, 1.0);
}