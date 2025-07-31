#version 460 core
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
       // Sun: emissive with 3D depth
        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - FragPos);
        
        // Create artificial lighting for 3D effect
        // Use view direction as a "light source" to create depth
        float ndotv = max(dot(norm, viewDir), 0.0);
        
        // Fresnel effect for rim lighting
        float fresnel = 1.0 - ndotv;
        
        // Create depth variation across the sphere
        float depth = ndotv * 0.4 + 0.6; // Vary from 0.6 to 1.0
        
        // Base emissive color with depth
        vec4 emissive = baseColor * depth * 1.2;
        
        // Add rim glow
        vec4 rimGlow = baseColor * fresnel * 0.8;
        
        // Add some surface variation (fake solar surface activity)
        float surface = sin(FragPos.x * 10.0) * sin(FragPos.y * 10.0) * sin(FragPos.z * 10.0) * 0.1 + 1.0;
        
        FragColor = (emissive + rimGlow) * surface;
    } else {
        // Planets: normal lighting
        vec3 norm = normalize(Normal);
        
        // Ambient lighting
        vec4 ambient = 0.15 * baseColor;
        
        // Diffuse lighting
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuse = diff * baseColor;
        
        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec4 specular = spec * vec4(1.0, 1.0, 1.0, 1.0);
        
        FragColor = ambient + diffuse + specular;
    }
}