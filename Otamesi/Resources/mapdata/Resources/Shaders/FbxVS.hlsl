#include "Fbx.hlsli"

//�X�L�j���O��̒��_�E�@��������
struct SkinOutput
{
	float4 pos;
	float3 normal;
};

//�X�L�j���O�̌v�Z
SkinOutput ComputeSkin(VSInput input)
{
	//�[���N���A
	SkinOutput output = (SkinOutput)0;

	uint iBone;//�v�Z����{�[���ԍ�
	float weight;//�{�[���̏d��
	matrix m;//�X�L�j���O�s��

	//�{�[��0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//iBone = input.boneIndices.x;
	//m = matSkinning[iBone];
	//output.pos = mul(m, input.pos);
	//output.normal = mul((float3x3)m, input.normal);

	return output;
}

VSOutput main(VSInput input)
{
	// �s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	float4 wnormal = float4(0, 0, 0, 0);

	if (isSkinning)
	{
		// �X�L�j���O�v�Z
		SkinOutput skinned = ComputeSkin(input);
		// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
		wnormal = normalize(mul(world, float4(skinned.normal, 0)));
		// �V�X�e�����W
		output.svpos = mul(mul(viewproj, world), skinned.pos);
		// ���C�g�p���W
		output.worldpos = mul(world, skinned.pos);
	}
	else
	{
		// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
		wnormal = normalize(mul(world, float4(input.normal, 0)));
		// �V�X�e�����W
		output.svpos = mul(mul(viewproj, world), input.pos);
		// ���C�g�p���W
		output.worldpos = mul(world, input.pos);
	}

	output.normal = wnormal.xyz;
	output.uv = input.uv;

	return output;
}