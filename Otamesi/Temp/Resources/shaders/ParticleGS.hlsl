#include "Particle.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0, 0), //����
	float4(-0.5f, +0.5f, 0, 0), //����
	float4(+0.5f, -0.5f, 0, 0), //�E��
	float4(+0.5f, +0.5f, 0, 0), //�E��
};

//���オ0,0�@�E����1,1
static const float2 uv_array[vnum] =
{
	float2(0,1), //����
	float2(0,0), //����
	float2(1,1), //�E��
	float2(1,0), //�E��
};

//�_�̓��͂���A�l�p�`���o��
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//4�_���܂킷
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset = offset_array[i] * input[0].scale;
		//���S����̃I�t�Z�b�g���r���{�[�h��](���f�����W)
		offset = mul(matBillboard, offset);

		//�I�t�Z�b�g�����炷(���[���h���W)
		element.svpos = input[0].pos + offset;
		//�r���[�v���W�F�N�V�����s��
		element.svpos = mul(mat, element.svpos);

		//��]�p���烉�W�A�����Z�o
		const float PI = 3.14159265359f;
		float radian = (input[0].rot * PI / 180);
		float s = sin(radian);
		float c = cos(radian);
		float2x2 rotMatrix = float2x2(c, -s, s, c);
		//uv�l�����炵�ĉ�]������
		float2 uv = uv_array[i] - float2(0.5f, 0.5f);
		uv = mul(rotMatrix, uv);
		uv += float2(0.5f, 0.5f);
		element.uv = uv;

		element.color = input[0].color;
		output.Append(element);
	}
}