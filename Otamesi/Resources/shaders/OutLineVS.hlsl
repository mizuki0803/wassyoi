#include "OutLine.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);

	//�s�N�Z���V�F�[�_�ɓn���l
    VSOutput output;
    output.svpos = mul(mul(viewproj, world), pos);

    output.worldpos = /*wpos*/float4(world._41_42_43_44);
    output.normal = wnormal.xyz;
    output.uv = uv;

	//���C�g���_���猩�����W
    output.shadowpos = mul(mul(light_viewproj, world), pos);

    return output;
}