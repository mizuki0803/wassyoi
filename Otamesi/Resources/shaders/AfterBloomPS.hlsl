#include "AfterBloom.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー


float4 main(VSOutput input) : SV_TARGET
{
	////ラジアンブラー
	//if (isRadialBlur) {
	//	//最終的に使用する色
	//	float4 color = { 0, 0, 0, 1 }; //黒で初期化
	//	//中心座標
	//	const float2 center = { 0.5f, 0.5f };
	//	//中心を基準にしておく
	//	float2 pos = input.uv - center;
	//	//基準からの距離
	//	float dist = length(pos);
	//	//サンプル回数
	//	float factor = radialBlurStrength / (float)radialBlurSampleNum * dist;
	//	for (int i = 0; i < (int)radialBlurSampleNum; i++) {
	//		float uvOffset = 1.0f - factor * float(i);
	//		color += tex.Sample(smp, pos * uvOffset + center);
	//	}
	//	//平均を求める
	//	color /= float(radialBlurSampleNum);
	//	return color;
	//}

    float4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
    float2 move_pixel = 1.0f / float2(1280.0f, 720.0f);
	
    color = tex.Sample(smp, input.uv);
	
	// 正方向
    for (int i = 1; i < 3;++i)
    {
        color += tex.Sample(smp, input.uv + move_pixel * float2(float(i), 0.0f));
        color += tex.Sample(smp, input.uv + move_pixel * float2(0.0f, float(i)));
        color += tex.Sample(smp, input.uv + move_pixel * float2(-float(i), 0.0f));
        color += tex.Sample(smp, input.uv + move_pixel * float2(0.0f, -float(i)));
    }
	
    color = color / (2.0f*4.0f + 1.0f);
	return color;
}