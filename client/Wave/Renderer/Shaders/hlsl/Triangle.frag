struct VSInput
{
    [[vk::location(0)]] float3 Color : COLOR;
};

float4 main(VSInput input) : SV_Target
{
    return float4(input.Color, 1.0);
}