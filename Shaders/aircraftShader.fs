#version 330 core

struct Material {
    sampler2D diffuse; 
    sampler2D specular; 
    float shininess; 
}; 

struct Light {
    vec3 position; 
    
    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 
}; 


in VS_OUT 
{
	vec3 Normal; 
	vec3 FragPos;
	vec2 TexCoords; 
	
} fs_in; 

out vec4 color; 

uniform vec3 viewPos;
uniform Material material; 
uniform Light light; 

uniform sampler2D texture_diffuse1; 

void main()
{
    //Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords)); 
    
    // Diffuse
    vec3 norm = normalize(fs_in.Normal); 
    vec3 lightDir = normalize(light.position - fs_in.FragPos); 
    float diff = max (dot(norm, lightDir), 0.0); 
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords)); 
     
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos); 
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow (max(dot(viewDir, reflectDir), 0.0), material.shininess); 
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords)); 
   
    color = vec4(ambient + diffuse + specular, 1.0f); 
}
