#include "HeightMap.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(normal, 0)));

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mul(viewproj, world), pos);
	output.worldpos = mul(world, pos);
	output.localpos = pos;
	output.normal = wnormal.xyz;
	output.uv = uv;

	return output;
}