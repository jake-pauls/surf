#version 450

layout(location = 0) in vec3 in_FragColor;
layout(location = 1) in vec2 in_TexCoord;

layout(location = 0) out vec4 o_Color;

layout(set = 1, binding = 0) uniform sampler2D u_Sampler;

void main()
{
    vec3 color = texture(u_Sampler, in_TexCoord).xyz;

    o_Color = vec4(color, 1.0f);
}