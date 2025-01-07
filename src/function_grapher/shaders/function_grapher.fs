#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 rgbaColor;

void main()
{
    FragColor = rgbaColor;
}
