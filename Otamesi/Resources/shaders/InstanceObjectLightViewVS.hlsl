#include "InstanceObjectLightView.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint instNo : SV_InstanceID)
{
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world[instNo], float4(normal, 0)));
    float4 wpos = mul(world[instNo], pos);

	//�s�N�Z���V�F�[�_�ɓn���l
    VSOutput output;
    output.svpos = mul(mul(viewproj, world[instNo]), pos);

    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    
    //�C���X�^���X�ԍ�
    output.instNo = instNo;

    return output;
}