#version 450

layout(location = 0) in vec3 in_FragColor;

layout(location = 0) out vec4 o_Color;

void main()
{
    o_Color = vec4(in_FragColor, 1.0);
}