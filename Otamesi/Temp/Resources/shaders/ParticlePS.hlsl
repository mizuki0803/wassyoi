#include "Particle.hlsli"

Texture2D<float4> tex : register(t0);	//0番テクスチャ
SamplerState smp : register(s0);		//0番サンプラー

float4 main(GSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv) * input.color;
	return texcolor;
}