#version 330 core 

in vec2 texcor; 
in vec4 rgba_;

uniform int hasTexture;

uniform sampler2D sempler; 

out vec4 color; 

void main()
{
	if (hasTexture < 0)
	{
		color  = vec4(0.0f, 0.0f, 0.0f, rgba_.w); 
	}
	else
	{
		color = texture(sempler, texcor);
	}
	
}

