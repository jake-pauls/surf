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

VSOutput main(VSInput input)
{
	VSOutput output = (VSOutput) 0;

	output.Position = float4(input.Position, 1.0);
	output.Color = input.Color;

	return output;
}