struct VSInput
{
    [[vk::location(0)]] float3 Color : COLOR;
    [[vk::location(1)]] float2 TexCoord : TEXCOORD;
};

[[vk::binding(0, 1)]]
Texture2D u_Texture;

[[vk::binding(0, 1)]]
SamplerState u_Sampler;

float4 main(VSInput input) : SV_Target
{
    float3 color = u_Texture.Sample(u_Sampler, input.TexCoord).xyz;

    return float4(color, 1.0);
}