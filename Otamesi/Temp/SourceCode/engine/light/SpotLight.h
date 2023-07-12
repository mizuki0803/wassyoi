#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

/// <summary>
/// �X�|�b�g���C�g
/// </summary>
class SpotLight
{
private://�G�C���A�X
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR lightv;
		Vector3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};

public://�����o�֐�

	//getter
	inline const XMVECTOR& GetLightDir() { return lightdir; }
	inline const Vector3& GetLightPos() { return lightpos; }
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }
	inline const XMFLOAT3& GetLightAtten() { return lightatten; }
	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos; }
	inline const bool GetActive() { return active; }

	//setter
	inline void SetLightDir(const XMVECTOR& lightdir) { this->lightdir = DirectX::XMVector3Normalize(lightdir); }
	inline void SetLightPos(const Vector3& lightpos) { this->lightpos = lightpos; }
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }
	inline void SetLightAtten(const XMFLOAT3& lightatten) { this->lightatten = lightatten; }
	inline void SetLightFactorAngleCos(const XMFLOAT2& lightFactorAngleCos) {
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngleCos.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngleCos.y));
	}
	inline void SetActive(bool active) { this->active = active; }


private://�����o�ϐ�
	//���C�g����(�P�ʃx�N�g��)
	XMVECTOR lightdir = { 1, 0, 0, 0 };
	//���C�g���W(���[���h���W�n)
	Vector3 lightpos = { 0, 0, 0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1, 1, 1 };
	//���C�g���������W��
	XMFLOAT3 lightatten = { 1.0f, 1.0f, 1.0f };
	//���C�g�����p�x
	XMFLOAT2 lightFactorAngleCos = { 0.5f, 0.2f };
	//�L���t���O
	bool active = false;
};