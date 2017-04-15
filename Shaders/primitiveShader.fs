#version 330 core 

in vec2 TexCoord; 

out vec4 color; 

uniform sampler2D terrainTexture; 

void main()
{
    color = texture(terrainTexture, TexCoord); 
	if (color.a < 0.2)
	    discard; 
    //color = vec4( 1.0f, 1.0f, 1.0f, .40f); 
}
