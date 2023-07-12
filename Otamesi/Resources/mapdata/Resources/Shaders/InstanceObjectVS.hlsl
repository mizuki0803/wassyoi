#include "InstanceObject.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint instNo : SV_InstanceID)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(matWorld[instNo], float4(normal, 0)));

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mul(viewproj, matWorld[instNo]), pos);
	output.worldpos = mul(matWorld[instNo], pos);
	output.normal = wnormal.xyz;
	output.uv = uv;
	output.instNo = instNo;

	return output;
}