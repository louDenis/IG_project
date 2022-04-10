#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 cameraPos ;

void main()
{
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 view = normalize(cameraPos - FragPos);
	vec3 lp = reflect(lightDir, Normal);
	float specular = pow(max(dot(-view, lp), 0.0f), 16);
    vec3 result = (specular + diffuse + ambient) * (Normal * 0.5 + 0.5);
    FragColor = vec4(result, 1.0);
} 