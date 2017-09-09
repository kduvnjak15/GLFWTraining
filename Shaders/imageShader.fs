#version 330 core 

in vec2 texcor; 

uniform sampler2D sempler; 

out vec4 color; 

void main()
{
	color = texture(sempler, texcor);
	//color  = vec4(1.0f, 0.0f, 1.0f, 1.0f); 
	
}

