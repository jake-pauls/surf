#version 450

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_Color;

layout(location = 0) out vec3 o_FragColor;

layout(push_constant) uniform constants 
{
    vec4 m_Data;
    mat4 m_MvpMatrix;
} u_PushConstants;

void main()
{
    gl_Position = u_PushConstants.m_MvpMatrix * vec4(in_Position, 1.0f);

    o_FragColor = in_Color;
}