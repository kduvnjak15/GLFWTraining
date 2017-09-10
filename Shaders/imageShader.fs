#version 330 core 

in vec2 texcor; 

uniform int hasTexture;
uniform vec4 rgba_;

uniform sampler2D sempler; 

out vec4 color; 

void main()
{
	if (hasTexture < 0)
	{
		color  = vec4(rgba_.x, rgba_.y, rgba_.z, 1.0f); 
	}
	else
	{
		color = texture(sempler, texcor);
	}
	
}

