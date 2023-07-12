#include "InstanceObjectLightView.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint instNo : SV_InstanceID)
{
	//法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world[instNo], float4(normal, 0)));
    float4 wpos = mul(world[instNo], pos);

	//ピクセルシェーダに渡す値
    VSOutput output;
    output.svpos = mul(mul(viewproj, world[instNo]), pos);

    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    
    //インスタンス番号
    output.instNo = instNo;

    return output;
}