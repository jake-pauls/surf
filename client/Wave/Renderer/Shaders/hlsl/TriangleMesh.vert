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

// Contains the required push constant data
struct PushData 
{
	float3 m_Data;
	row_major float4x4 m_MvpMatrix;
};

// Binds the push constant sent from Vulkan
[[vk::push_constant]]
PushData pcs;

VSOutput main(VSInput input)
{
	VSOutput output = (VSOutput) 0;

	output.Position = mul(float4(input.Position, 1.0), pcs.m_MvpMatrix);
	output.Color = input.Color;

	return output;
}