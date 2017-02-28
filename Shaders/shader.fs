#version 330 core

in vec2 texCoord;

uniform sampler2D texSampler;

out vec4 color;

void main()
{
        color = texture(texSampler,texCoord);
}


