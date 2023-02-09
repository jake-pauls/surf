struct VSOutput
{
	float4 Position : SV_POSITION;
    [[vk::location(0)]] float3 Color : COLOR0;
};

VSOutput main(uint VertexIndex : SV_VertexID)
{
	float2 positions[3] = {
		float2( 0.0, -0.5),
		float2( 0.5,  0.5),
		float2(-0.5,  0.5)
	};

	float3 colors[3] = {
		float3(1.0, 0.0, 0.0),
		float3(0.0, 1.0, 0.0),
		float3(0.0, 0.0, 1.0)
	};

	VSOutput output = (VSOutput) 0;

	output.Position = float4(positions[VertexIndex], 0.0, 1.0);
	output.Color = colors[VertexIndex];

	return output;
}