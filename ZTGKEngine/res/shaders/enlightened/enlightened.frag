#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;

uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

uniform float LightConstant;
uniform float LightLinear;
uniform float LightQuadratic;

uniform bool isHoovered;


void main()
{
	//ambient
        float ambientPointStrength = 0.1;
	    vec3 ambient = (ambientPointStrength * pointLightColor);

	//diffuse
    	vec3 norm = normalize(Normal);
    	vec3 lightDir = normalize(pointLightPos - FragPos);
    	float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = (diff * pointLightColor);

	//specular
    	float specularPointStrength = 0.9;
    	vec3 viewDir = normalize(viewPos - FragPos); 
        vec3 halfwayDir = normalize(lightDir + viewDir);   
    	float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
    	vec3 specular = (specularPointStrength * spec * pointLightColor);

    
    // attenuation
        float distance    = length(pointLightPos - FragPos);
        float attenuation = 1.0 / (LightConstant + LightLinear * distance + LightQuadratic * (distance * distance)); 
        if(pointLightColor.r < 0.5)
        {
            attenuation *= 0.8;
        }

        ambient  *= attenuation;  
        specular *= attenuation;
        diffuse  *= attenuation;

    	vec3 result = (ambient + specular) + (diffuse);
        if(isHoovered)
        {
            FragColor = texture(texture_diffuse1, TexCoords) * vec4(result, 1.0) + vec4(0.2, 0.2, 0.2, 0.0);
        }
        else
        {
            FragColor = texture(texture_diffuse1, TexCoords) * vec4(result, 1.0);
        }
    	
}