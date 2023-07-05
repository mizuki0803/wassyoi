#include "AfterBloom.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


float4 main(VSOutput input) : SV_TARGET
{
	////���W�A���u���[
	//if (isRadialBlur) {
	//	//�ŏI�I�Ɏg�p����F
	//	float4 color = { 0, 0, 0, 1 }; //���ŏ�����
	//	//���S���W
	//	const float2 center = { 0.5f, 0.5f };
	//	//���S����ɂ��Ă���
	//	float2 pos = input.uv - center;
	//	//�����̋���
	//	float dist = length(pos);
	//	//�T���v����
	//	float factor = radialBlurStrength / (float)radialBlurSampleNum * dist;
	//	for (int i = 0; i < (int)radialBlurSampleNum; i++) {
	//		float uvOffset = 1.0f - factor * float(i);
	//		color += tex.Sample(smp, pos * uvOffset + center);
	//	}
	//	//���ς����߂�
	//	color /= float(radialBlurSampleNum);
	//	return color;
	//}

    float4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
    float2 move_pixel = 1.0f / float2(1280.0f, 720.0f);
	
    color = tex.Sample(smp, input.uv);
	
	// ������
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