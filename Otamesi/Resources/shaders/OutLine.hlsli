// �J�����o�b�t�@
cbuffer CONSTANT_BUFFER_CAMERA : register(b0)
{
    row_major float4x4 view_projection; // �r���[�v���W�F�N�V����
    row_major float4x4 inv_view_projection; // �r���[�v���W�F�N�V�����t�s��
};


//���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 viewprojection_position : SV_POSITION; // VP���W
    float4 world_position : W_POSITION; // ���[���h���W���W
	float2 uv : TEXCOORD;	//uv�l
};

struct PSOutput
{
	float4 color				: SV_TARGET0;	// �F�摜
	float4 depth				: SV_TARGET1;	// �[�x�摜
};