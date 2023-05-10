#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

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
	//���W�A���u���[
	if (isRadialBlur) {
		//�ŏI�I�Ɏg�p����F
		float4 color = { 0, 0, 0, 1 }; //���ŏ�����
		//���S���W
		const float2 center = { 0.5f, 0.5f };
		//���S����ɂ��Ă���
		float2 pos = input.uv - center;
		//�����̋���
		float dist = length(pos);
		//�T���v����
		float factor = radialBlurStrength / (float)radialBlurSampleNum * dist;
		for (int i = 0; i < (int)radialBlurSampleNum; i++) {
			float uvOffset = 1.0f - factor * float(i);
			color += tex.Sample(smp, pos * uvOffset + center);
		}
		//���ς����߂�
		color /= float(radialBlurSampleNum);
		return color;
	}
	
	//�ʏ�
	float4 texColor = tex.Sample(smp, input.uv);
	return float4(texColor.rgb, 1);
}