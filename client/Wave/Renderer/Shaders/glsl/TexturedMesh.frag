#version 450

layout(location = 0) in vec3 in_FragColor;
layout(location = 1) in vec2 in_TexCoord;

layout(location = 0) out vec4 o_Color;

layout(set = 1, binding = 0) uniform sampler2D u_Sampler;

layout(binding = 0) uniform UniformBufferObject
{
    // PBR
    vec4 m_LightPosition;
    vec4 m_LightColor;
    vec4 m_Albedo;
    vec4 m_PBRSettings;
    // View
    mat4 m_ViewMatrix;
    mat4 m_ProjectionMatrix;
} u_UBO;

void main()
{
    vec3 albedo = u_UBO.m_Albedo.xyz;

    vec3 color = texture(u_Sampler, in_TexCoord).xyz * albedo;

    o_Color = vec4(color, 1.0f);
}