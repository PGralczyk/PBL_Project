#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{             
    vec3 originalColor = texture(scene, TexCoords).rgb;     
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    vec3 proportion = bloomColor / vec3(1.0f);
    //originalColor += bloomColor; 
    // tone mapping
    //vec3 result = vec3(1.0) - exp(-originalColor * exposure);
    vec3 result = originalColor * proportion + bloomColor * (vec3(1.0f) - proportion);
    // also gamma correct while we're at it       
    FragColor = vec4(result, 1.0);
}