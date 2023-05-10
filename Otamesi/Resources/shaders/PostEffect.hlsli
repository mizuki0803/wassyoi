cbuffer cbuff0 : register(b0)
{
	uint isRadialBlur; //���W�A���u���[�������邩
	uint radialBlurSampleNum; //���W�A���u���[�̃T���v����
	float radialBlurStrength; //���W�A���u���[�̍L���鋭��
};

//���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float2 uv : TEXCOORD;	//uv�l
};
