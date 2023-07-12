cbuffer cbuff0:register(b0)
{
	matrix viewproj; // �r���[�v���W�F�N�V�����s��
	matrix world; // ���[���h�s��
	float3 cameraPos; // �J�������W�i���[���h���W�j
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float3 uv : TEXCOORD;
};