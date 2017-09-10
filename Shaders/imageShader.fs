#version 330 core 

in vec2 texcor; 

uniform int hasTexture;
uniform float transparency;
uniform vec4 rgba_;
uniform float brightness;

uniform sampler2D sempler; 

out vec4 color; 

void main()
{
	float b = abs(brightness);
	float t = transparency;
	
	if (hasTexture < 0)
	{
		color  = vec4(rgba_.x, rgba_.y, rgba_.z, rgba_.z); 
	}
	else
	{
		color = texture(sempler, texcor);
	}
	
	color = vec4(b*color.x, b*color.y, b*color.z, t*color.w);
	
}

