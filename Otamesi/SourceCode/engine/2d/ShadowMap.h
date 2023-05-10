#pragma once
#include <DirectXMath.h>
#include "Texture.h"

/// <summary>
/// �V���h�E�}�b�v
/// </summary>
class ShadowMap
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
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color;	//�F(RGBA)
		XMMATRIX mat;	//3�ϊ��s��
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// �V���h�E�}�b�v����
	/// </summary>
	/// <returns>�V���h�E�}�b�v</returns>
	static ShadowMap* Create();

	/// <summary>
	/// �V���h�E�}�b�v���ʕ����̏�����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void ShadowMapCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	void DrawScenePrev();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	void DrawSceneRear();

	//getter
	const Texture& GetTexture() { return depthTexture; }

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�V���h�E�}�b�v�e�N�X�`���T�C�Y(4K)
	static const int shadowMapTexSize = 4096;

private: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�e�N�X�`��
	Texture depthTexture;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};

