#pragma once
#include <DirectXMath.h>
#include "PipelineSet.h"
#include "Texture.h"

/// <summary>
/// �|�X�g�G�t�F�N�g
/// </summary>
class PostEffect
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�T�u�N���X
	struct VertexPosUv
	{
		XMFLOAT3 pos;	//xyz���W
		XMFLOAT2 uv;	//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		unsigned int isRadialBlur; //���W�A���u���[�������邩
		unsigned int radialBlurSampleNum; //���W�A���u���[�̃T���v����
		float radialBlurStrength; //���W�A���u���[�̍L���鋭��
		float pad1;
	};

public: //�萔
	//���W�A���u���[�̍ŏ��T���v����
	static const int radialBlurSampleNumMin = 1;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �|�X�g�G�t�F�N�g����
	/// </summary>
	/// <returns>�|�X�g�G�t�F�N�g</returns>
	static PostEffect* Create();

	/// <summary>
	/// �|�X�g�G�t�F�N�g���ʕ����̏�����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void PostEffectCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	void DrawScenePrev();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	void DrawSceneRear();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipelineState();

	//getter
	const bool GetIsRadialBlur() { return isRadialBlur; }
	const int GetRadialBlurSampleNum() { return radialBlurSampleNum; }
	const float GetRadialBlurStrength() { return radialBlurStrength; }

	//setter
	void SetRadialBlur(const bool isRadialBlur) { this->isRadialBlur = isRadialBlur; }
	void SetRadialBlurSampleNum(const int radialBlurSampleNum) { this->radialBlurSampleNum = radialBlurSampleNum; }
	void SetRadialBlurStrength(const float radialBlurStrength) { this->radialBlurStrength = radialBlurStrength; }

private: //�ÓI�����o�ϐ�
	//��ʃN���A�J���[
	static const float clearColor[4];
	//�f�o�C�X
	static ID3D12Device* dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C���Z�b�g
	static PipelineSet pipelineSet;

private: //�����o�ϐ�
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�e�N�X�`��
	Texture texture;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�F(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//���W�A���u���[�������邩
	bool isRadialBlur = false;
	//���W�A���u���[�̃T���v����
	int radialBlurSampleNum = 10;
	//���W�A���u���[�̍L���鋭��
	float radialBlurStrength = 0.5f;
};

