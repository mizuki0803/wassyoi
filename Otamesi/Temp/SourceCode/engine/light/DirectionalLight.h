#pragma once
#include <DirectXTex.h>

/// <summary>
/// ���s����
/// </summary>
class DirectionalLight
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR lightv;		//���C�g�ւ̕�����\���x�N�g��
		XMFLOAT3 lightcolor;	//���C�g�̐F
		unsigned int active;	//�L���t���O
	};


public: //�����o�֐�

	//getter
	inline const XMVECTOR& GetLightDir() { return lightdir; }
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }
	inline const bool GetActive() { return active; }

	//setter
	inline void SetLightDir(const XMVECTOR& lightdir) { this->lightdir = DirectX::XMVector3Normalize(lightdir); }
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }
	inline void SetActive(bool active) { this->active = active; }

private: // �����o�ϐ�
	//���C�g��������(�P�ʃx�N�g��)
	XMVECTOR lightdir = { 1,0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//�L���t���O
	bool active = false;
};