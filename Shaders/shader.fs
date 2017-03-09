#version 330 core 

out vec4 color; 

uniform vec3 objectColor; 
uniform vec3 lightColor; 

uniform float uniformS_; 



void main()
{
	vec3 temp = vec3(uniformS_, uniformS_, uniformS_); 
	
	color = vec4(temp * objectColor, 1.0f); 
}
