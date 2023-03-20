struct VSInput
{
    [[vk::location(0)]] float3 Position : SV_POSITION;
    [[vk::location(1)]] float3 Normal : NORMAL;
    [[vk::location(2)]] float3 Color : COLOR0; 
};

struct VSOutput
{
	float4 Position : SV_POSITION;
    [[vk::location(0)]] float3 Color : COLOR0;
};

struct UniformBufferObject
{
    row_major float4x4 m_ViewMatrix;
    row_major float4x4 m_ProjectionMatrix;
};

struct PushConstants 
{
	row_major float4x4 m_ModelMatrix;
};

[[vk::binding(0, 0)]]
cbuffer u_UBO 
{
	UniformBufferObject u_UBO;
}

[[vk::push_constant]]
PushConstants u_PCS;

VSOutput main(VSInput input)
{
	VSOutput output = (VSOutput) 0;

	float4x4 cameraMatrix = mul(u_UBO.m_ViewMatrix, u_UBO.m_ProjectionMatrix);
	float4x4 transformMatrix = mul(u_PCS.m_ModelMatrix, cameraMatrix);

	output.Position = mul(float4(input.Position, 1.0), transformMatrix);
	output.Color = input.Color;

	return output;
}