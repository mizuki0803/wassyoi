#pragma once
#include "Texture.h"
#include <wrl.h>
#include <d3d12.h>

/// <summary>
/// SRV�p�f�X�N���v�^�q�[�v
/// </summary>
class DescHeapSRV
{
private: //�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�e�N�X�`���̍ő喇��
	static const int SRVCount = 512;

public: //�ÓI�����o�֐�
	/// <summary>
	/// ���ʏ������ݒ�
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �S�V�[�����ʂŎg�p����e�N�X�`���������m�肳����
	/// </summary>
	static void SetAllSceneTextureNum();

	/// <summary>
	/// �V�[���؂�ւ����ɑS�V�[�����ʃe�N�X�`���ȊO�̕��C���f�b�N�X��߂�
	/// </summary>
	static void TextureDestruction();

	/// <summary>
	/// SRV����SetDescriptorHeaps
	/// </summary>
	static void SetDescriptorHeaps();

	/// <summary>
	/// SRV����CreateShaderResourceView
	/// </summary>
	static void CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc, Texture& texture);

	/// <summary>
	/// SRV����SetGraphicsRootDescriptorTable
	/// </summary>
	static void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber);

public: //�ÓI�����o�ϐ�  (��������g���̂ŁA�g���₷���悤��public�ɂ��Ă���)
	//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeapSRV;

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�S�V�[�����ʂŎg�p����e�N�X�`���̖���
	static UINT allSceneTextureNum;
	//�e�N�X�`���ԍ��J�E���^�[
	static UINT texNumCount;
};