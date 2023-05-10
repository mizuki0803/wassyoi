#pragma once
#include <wrl.h>
#include <d3d12.h>

/// <summary>
/// �e�N�X�`��
/// </summary>
struct Texture
{
private: //�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //�����o�ϐ�
	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)
	ComPtr<ID3D12Resource> texBuff;
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
};