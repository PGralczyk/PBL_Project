#version 330 core
out vec4 FragColor;

//out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

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
    float bias = 0.005; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

	//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);  
    //FragColor = vec4(closestDepth, 1.0, 1.0, 1.0);
	
    return shadow;
}


void main()
{
		vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	//ambient
        float ambientPointStrength = 0.1;
	    vec3 ambient = (ambientPointStrength * color);

	//diffuse
    	vec3 norm = normalize(fs_in.Normal);
    	vec3 lightDir = normalize(pointLightPos - fs_in.FragPos);
    	float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = (diff * pointLightColor);

	//specular
    	float specularPointStrength = 0.9;
    	vec3 viewDir = normalize(viewPos - fs_in.FragPos); 
        vec3 halfwayDir = normalize(lightDir + viewDir);   
    	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64);
    	vec3 specular = (specularPointStrength * spec * pointLightColor);

    
    // attenuation
        float distance    = length(pointLightPos - fs_in.FragPos);
        float attenuation = 1.0 / (LightConstant + LightLinear * distance + LightQuadratic * (distance * distance)); 
        if(pointLightColor.r < 0.5)
        {
            attenuation *= 0.8;
        }

        ambient  *= attenuation;  
        specular *= attenuation;
        diffuse  *= attenuation;
		
	// calculate shadow
	   float shadow = ShadowCalculation(fs_in.FragPos);
       vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	   //(1.0 - shadow) *
       if(isHoovered)
       {
          FragColor =  vec4(result, 1.0) + vec4(0.2, 0.2, 0.2, 0.0);
       }
       else
       {
          FragColor = vec4(result, 1.0);
       }
    	
}