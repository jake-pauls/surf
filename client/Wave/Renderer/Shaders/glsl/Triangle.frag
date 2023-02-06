#version 450

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec3 in_FragColor;

void main()
{
    o_Color = vec4(in_FragColor, 1.0);
}