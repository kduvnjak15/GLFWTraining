#version 330 core 

out vec4 color; 

uniform int CPUcolor; 

void main()
{
    if (CPUcolor == 1)
        color = vec4( 1.00f, .00f, .000f, 1.0f); 
    else if (CPUcolor == 2)
        color = vec4( 0.00f, 0.10f, 1.0f, 1.0f); 
    else if (CPUcolor == 3)
        color = vec4( 1.00f, 0.0f, 0.50f, 1.0f); 
    else 
        color = vec4( 0.10f, 0.10f, 0.10f, 0.6f); 
}
