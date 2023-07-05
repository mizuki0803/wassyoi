#include "OutLine.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//ピクセルシェーダに渡す値
	output.uv = uv;
	
    output.world_position = float4(pos.xyz, 1.0f);
    output.viewprojection_position = mul(float4(pos.xyz, 1.0f), output.world_position);
	
	return output;
}