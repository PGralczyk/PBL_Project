#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D highlightParts;
uniform sampler2D noise;
uniform float strength;


void main()
{             
    vec3 originalColor = texture(scene, TexCoords).rgb;     
    vec3 highlightColor = texture(highlightParts, TexCoords).rgb;
    vec3 noiseColor = texture(noise, TexCoords).rgb;
    
    FragColor = vec4(originalColor + highlightColor * strength + noiseColor, 1.0);
}