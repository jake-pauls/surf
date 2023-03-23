struct VSInput
{
    [[vk::location(0)]] float3 Color : COLOR;
    [[vk::location(1)]] float2 TexCoord : TEXCOORD;
};

struct UniformBufferObject
{
	// PBR
	float4 m_LightPosition;
	float4 m_LightColor;
	float4 m_Albedo;
	float4 m_PBRSettings;
	// View
    row_major float4x4 m_ViewMatrix;
    row_major float4x4 m_ProjectionMatrix;
};

[[vk::binding(0, 1)]]
Texture2D u_Texture;

[[vk::binding(0, 1)]]
SamplerState u_Sampler;

[[vk::binding(0, 0)]]
cbuffer u_UBO { UniformBufferObject u_UBO; }

float4 main(VSInput input) : SV_Target
{
    float3 albedo = u_UBO.m_Albedo.xyz;
    float3 color = u_Texture.Sample(u_Sampler, input.TexCoord).xyz * albedo;

    return float4(color, 1.0);
}