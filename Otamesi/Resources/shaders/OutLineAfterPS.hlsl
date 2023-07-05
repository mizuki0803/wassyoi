#include "OutLineAfter.hlsli"

Texture2D color_texture_2d : register(t0);
Texture2D depth_texture_2d : register(t1);

SamplerState texture_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = color_texture_2d.Sample(texture_sampler, input.uv);
    float4 depth = depth_texture_2d.Sample(texture_sampler, input.uv);
    
    return color;
    //return float4(0.0f,0.0f,1.0f,1.0f); // 1枚のターゲットに書き込み
}