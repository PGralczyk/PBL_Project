#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;


void main()
{             
    vec4 color = texture(scene, TexCoords);
    float brighteness = (color.r * 0.2126) + (color.g * 0.7152)+ (color.b * 0.0722);
    //FragColor = color * brighteness * brighteness;
    if(brighteness > 0.8){
        FragColor = color;  
    }else {
        FragColor = vec4(0.0);
    }
}