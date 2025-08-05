#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; 

uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection; 
uniform mat4 scale; 
//uniform mat4 rotate; 

out vec3 FragPos; 
out vec3 Normal; 
void main()
{
   FragPos = vec3(model*vec4(aPos, 1.0f)); 
   Normal = mat3(transpose(inverse(model))) * aNormal; 
	
   gl_Position =  projection * view * model * vec4(aPos, 1.0f);

}


