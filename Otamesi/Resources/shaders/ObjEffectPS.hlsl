#include "ObjEffect.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> shadowMap : register(t1);	//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[



float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
    float4 output_color = tex.Sample(smp, input.uv) * color;
	
	// �V�F�[�f�B���O�ɂ��F�ŕ`��;
    return float4(output_color.rgb /*/ (4 * 4)*/, color.a);

}