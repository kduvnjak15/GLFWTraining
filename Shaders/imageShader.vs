#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 texpos;
layout (location = 2) in vec4 color; 

out vec2 texcor; 
out vec4 rgba_;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
   texcor = texpos; 
   rgba_ = color;
}
