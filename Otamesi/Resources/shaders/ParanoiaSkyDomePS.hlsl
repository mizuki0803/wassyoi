#include "ParanoiaSkyDome.hlsli"

Texture2D<float4> tex:register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);	//0番スロットに設定されたサンプラー

float2 EquirectangularProjection(float3 v)
{
    float2 texcoord;
    texcoord = float2(1.0f - atan2(v.z, v.x) / 2.0f, -atan2(v.y, length(v.xz)));
    texcoord = texcoord / 3.1415926535f+ 0.5f;
    return texcoord;
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 w_position = float4(
        input.uv.x * 2.0f - 1.0f,
        -input.uv.y * 2.0f + 1.0f,
        0,
		1);

    w_position = mul(w_position, inv_view_projection);
    w_position = w_position / w_position.w;

    float3 v = w_position.xyz - camera_position.xyz;

    float2 uv = EquirectangularProjection(normalize(v));

	return float4(tex.Sample(smp, uv).rgb, 1);
}