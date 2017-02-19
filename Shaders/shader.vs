#version 330 core

layout (location = 0) in vec3 Position; 

out vec4 ccolor; 

void main()
{
    gl_Position = vec4(Position, 1.0); 

    ccolor = vec4(clamp(gl_Position, 0.0f, 1.0f)); 
}
