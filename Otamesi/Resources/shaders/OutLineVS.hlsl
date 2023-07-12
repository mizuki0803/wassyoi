#include "OutLine.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);

	//ピクセルシェーダに渡す値
    VSOutput output;
    output.svpos = mul(mul(viewproj, world), pos);

    output.worldpos = /*wpos*/float4(world._41_42_43_44);
    output.normal = wnormal.xyz;
    output.uv = uv;

	//ライト視点から見た座標
    output.shadowpos = mul(mul(light_viewproj, world), pos);

    return output;
}