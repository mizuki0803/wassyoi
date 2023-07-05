#include "OutLine.hlsli"

Texture2D color_texture_2d : register(t0);

SamplerState texture_sampler : register(s0);

PSOutput main(VSOutput input) : SV_TARGET
{
    PSOutput output;

    output.color = color_texture_2d.Sample(texture_sampler, input.uv);
    
    float depth = input.viewprojection_position.z / input.viewprojection_position.w;
    output.depth = float4(depth, depth, 1.0f, 1.0f);
    
    return output; // 2枚のターゲットに書き込み
}