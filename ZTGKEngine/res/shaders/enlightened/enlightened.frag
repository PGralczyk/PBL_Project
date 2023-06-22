#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform samplerCube depthMap;

uniform vec3 viewPos;
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

uniform float LightConstant;
uniform float LightLinear;
uniform float LightQuadratic;
uniform float far_plane;

uniform bool isHoovered;


float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - pointLightPos;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.015; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    //DUPpA DEBUGER BUT FOR SHADERS
	//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);  
    //FragColor = vec4(closestDepth, 1.0, 1.0, 1.0);
	
    return shadow;
}

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

        float shadow = ShadowCalculation(FragPos);
        vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));

        if(isHoovered)
        {
            FragColor = texture(texture_diffuse1, TexCoords) * vec4(result, 1.0) + vec4(0.2, 0.2, 0.2, 0.0);
        }
        else
        {
            FragColor = texture(texture_diffuse1, TexCoords) * vec4(result, 1.0);
        }
    	
}