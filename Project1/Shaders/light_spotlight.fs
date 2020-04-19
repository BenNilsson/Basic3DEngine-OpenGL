#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;
  
uniform vec3 viewPos; 
uniform Material material;
uniform Light light;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);

    // Firstly, calculate and check if the light is within the spotlight cone
	float theta = dot(lightDir, normalize(-light.direction));
	
	if(theta > light.cutOff)
	{
		// If inside the cone
		
		// Ambient
		vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
		
		// Diffuse
		vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 
		
		// Specular
		vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
		
		// Attenuation
		float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		// Note that in spotlights, we do not calculate attenuation in ambient as it would cause darker light outside of the spotlight
		diffuse *= attenuation;
		specular *= attenuation;
		
		// Result
		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);
	}else
	{
		// If the light is outside of the cone, use the ambient light so the scene isn't completely darker
		FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
	}
} 