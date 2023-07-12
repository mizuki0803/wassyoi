#include "ObjEffect.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> shadowMap : register(t1);	//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー



float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
    float4 output_color = tex.Sample(smp, input.uv) * color;
	
	// シェーディングによる色で描画;
    return float4(output_color.rgb /*/ (4 * 4)*/, color.a);

}