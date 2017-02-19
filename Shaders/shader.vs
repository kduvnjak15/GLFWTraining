#version 330 core

layout (location = 0) in vec3 Position

void main()
{
    glPosition = vec4(Position, 1.0); 
}
