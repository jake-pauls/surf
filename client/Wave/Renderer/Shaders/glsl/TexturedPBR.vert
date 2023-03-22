#version 450

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_Color;
layout(location = 3) in vec2 in_TexCoord;

layout(location = 0) out vec3 o_FragColor;
layout(location = 1) out vec2 o_TexCoord;
layout(location = 2) out vec3 o_WorldPosition;
layout(location = 3) out vec3 o_Normal;

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

layout(push_constant) uniform PushConstants 
{
    vec4 m_CameraPosition;
    mat4 m_ModelMatrix;
} u_PCS;

void main()
{
    mat4 transformMatrix = (u_UBO.m_ProjectionMatrix * u_UBO.m_ViewMatrix) * u_PCS.m_ModelMatrix;

    gl_Position = transformMatrix * vec4(in_Position, 1.0f);

    o_FragColor = in_Color;
    o_TexCoord = in_TexCoord;
    o_WorldPosition = vec3(u_PCS.m_ModelMatrix * vec4(in_Position, 1.0f));
    o_Normal = mat3(u_PCS.m_ModelMatrix) * in_Normal;
}