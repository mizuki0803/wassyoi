cbuffer cbuff0:register(b0)
{
	float4 outlineColor;//�A�E�g���C���̐F
	float outlineWidth;//�A�E�g���C���̑���
	unsigned int isFog;//�t�H�O�̗L��
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float2 uv : TEXCOORD;
};