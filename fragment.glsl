#version 460 core
out vec4 FragColor;
in vec3 FragPos; 
in vec3 Normal; 
uniform vec3 lightPos; 
uniform vec3 viewPos; 

void main()
{
    vec3 baseColor = vec3(0.8, 0.6, 0.4);  // Nice warm color
    vec3 norm = normalize(Normal);
    
    // Ambient lighting
    vec3 ambient = 0.15 * baseColor;
    
    // Diffuse lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * baseColor;
    
    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * vec3(1.0, 1.0, 1.0);
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}