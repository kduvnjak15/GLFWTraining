#version 330 core


uniform float katarina; 

in vec4 ccolor; 
out vec4 color; 

void main()
{
	color = vec4(katarina, 0.0f, 0.0f, 1.0f);

}


