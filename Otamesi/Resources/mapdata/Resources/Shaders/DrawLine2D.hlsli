cbuffer cbuff0:register(b0)
{
	float4 color;//�F(RGBA)
	matrix mat;//3D�ϊ��s��
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
};