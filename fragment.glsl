#version 420 core
out vec4 FragColor;
in vec3 FragPos; 
in vec3 Normal; 
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec4 objColor; 
uniform bool emitLight; 
void main()
{
    vec4 baseColor = objColor;
    vec3 norm = normalize(Normal);
    vec4 lighting; 
    if(emitLight){
        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - FragPos);

        float ndotv = max(dot(norm, viewDir), 0.0);
        
        float fresnel = 1.0 - ndotv;
         
        float depth = ndotv * 0.4 + 0.6; // Vary from 0.6 to 1.0
        
        vec4 emissive = baseColor * depth * 1.2;
       
        vec4 rimGlow = baseColor * fresnel * 0.8;
        
        float surface = sin(FragPos.x * 10.0) * sin(FragPos.y * 10.0) * sin(FragPos.z * 10.0) * 0.1 + 1.0;
        
        FragColor = (emissive + rimGlow) * surface;
    } else {
        vec3 norm = normalize(Normal);
        
        vec4 ambient = 0.15 * baseColor;
        
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuse = diff * baseColor;
        
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec4 specular = spec * vec4(1.0, 1.0, 1.0, 1.0);
        
        FragColor = ambient + diffuse + specular;
    }
}