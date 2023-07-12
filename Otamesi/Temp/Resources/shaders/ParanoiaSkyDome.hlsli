cbuffer cbuff0 : register(b0)
{
    float4 color; //�F(RGBA)
    matrix mat; //3D�ϊ��s��
}
cbuffer CameraBuffer : register(b1)
{
    float4 camera_position; //�F(RGBA)
    row_major float4x4	view_projection; //3D�ϊ��s��
    row_major float4x4	inv_view_projection; //3D�ϊ��s��

}

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�\�ւ̂����Ɏg�p����\����
struct VSOutput {
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float2 uv : TEXCOORD;	//uv�l
};