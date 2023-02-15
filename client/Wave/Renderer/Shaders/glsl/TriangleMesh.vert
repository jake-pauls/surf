#version 450

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_Color;

layout(location = 0) out vec3 o_FragColor;

void main()
{
    gl_Position = vec4(in_Position, 1.0);

    o_FragColor = in_Color;
}