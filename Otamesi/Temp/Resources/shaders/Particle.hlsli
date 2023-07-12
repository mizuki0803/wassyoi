cbuffer cbuff : register(b0)
{	
	matrix mat;	//�r���[�v���W�F�N�V�����s��
	matrix matBillboard;	//�r���{�[�h�s��
};

//���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 pos : POSITION;	//���_���W
	float scale : TEXCOORD;	//�X�P�[��
	float rot : ROTATION; //��]�p
	float4 color : COLOR;	//�F
};

//�W�I���g���V�F�[�_����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput
{
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float2 uv : TEXCOORD;		//uv�l
	float4 color : COLOR;	 //�F
};