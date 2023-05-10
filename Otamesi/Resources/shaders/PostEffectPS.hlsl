#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(float2 uv, float sigma, float stepWidth)
{
	float totalWeight = 0;
	float4 col = float4(0, 0, 0, 0);

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth) {
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth) {
			float2 pickUV = uv + float2(px, py);
			float weight = Gaussian(uv, pickUV, sigma);
			col += tex.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}
	col.rgb = col.rgb / totalWeight;
	return col;
}

float4 main(VSOutput input) : SV_TARGET
{
	//ラジアンブラー
	if (isRadialBlur) {
		//最終的に使用する色
		float4 color = { 0, 0, 0, 1 }; //黒で初期化
		//中心座標
		const float2 center = { 0.5f, 0.5f };
		//中心を基準にしておく
		float2 pos = input.uv - center;
		//基準からの距離
		float dist = length(pos);
		//サンプル回数
		float factor = radialBlurStrength / (float)radialBlurSampleNum * dist;
		for (int i = 0; i < (int)radialBlurSampleNum; i++) {
			float uvOffset = 1.0f - factor * float(i);
			color += tex.Sample(smp, pos * uvOffset + center);
		}
		//平均を求める
		color /= float(radialBlurSampleNum);
		return color;
	}
	
	//通常
	float4 texColor = tex.Sample(smp, input.uv);
	return float4(texColor.rgb, 1);
}