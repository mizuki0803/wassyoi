#pragma once
#include <Windows.h>
#include "DirectXBase.h"
#include <DirectXMath.h>
#include "PipelineSet.h"
#include "ObjModel.h"
#include "LightGroup.h"
#include "Camera.h"
#include "LightCamera.h"
#include "Vector3.h"
#include "ObjObject3d.h"
#include <array>

/// <summary>
/// �C���X�^���X�I�u�W�F�N�g
/// </summary>
class InstanceObject
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		Vector3 cameraPos;	//�J�������W(���[���h���W)
		XMMATRIX lightViewproj;	//���C�g�r���[�v���W�F�N�V�����s��
		unsigned int isShadowMap;	//�e��t���邩
	};

	//�萔�o�b�t�@�p�f�[�^�\����B0
	static const int draw_max_num = 512;
	struct ConstBufferDataB1
	{
		XMFLOAT4 color[draw_max_num];		//�F
		XMMATRIX world[draw_max_num];		//���[���h�s��
	};

	//�e�p
	struct ConstBufferDataLightViewB0
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		Vector3 cameraPos;	//�J�������W(���[���h���W)
	};

	struct ConstBufferDataLightViewB1
	{
		XMMATRIX world[draw_max_num];		//���[���h�s��
	};

	enum class PipelineType
	{
		Alpha,
		Add,
		Size,
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// Object3d���ʕ����̏�����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void InstanceObjectCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateAlphaPipeline();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateAddPipeline();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateLightViewPipeline();

	/// <summary>
	/// �`��O����
	/// </summary>
	static void DrawPrev(PipelineType _type = PipelineType::Alpha);

	/// <summary>
	/// �`��O����
	/// </summary>
	static void DrawLightViewPrev();

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>object3d</returns>
	static InstanceObject* Create(ObjModel* model);


public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	virtual bool Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��Z�b�g
	/// </summary>
	/// <param name="_pos">���W</param>
	/// <param name="_scale">�傫��</param>
	/// <param name="_rotation">��]�p</param>
	/// <param name="_color">�F</param>
	/// <param name="_parentWorld">�e�I�u�W�F�N�g���[���h�s��</param>
	void DrawInstance(const XMFLOAT3& _pos, const XMFLOAT3& _scale,
		const XMFLOAT3& _rotation, const XMFLOAT4& _color, const XMMATRIX* _parentWorld = nullptr);

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	virtual void DrawLightCameraView();

	//setter
	void SetModel(ObjModel* model) { this->model = model; }
	void SetIsCameraFollow(bool isCameraFollow) { this->isCameraFollow = isCameraFollow; }
	void SetIsShadowMap(bool isShadowMap) { this->isShadowMap = isShadowMap; }
	static void SetLightGroup(LightGroup* lightGroup) { InstanceObject::lightGroup = lightGroup; }
	static void SetCamera(Camera* camera) { InstanceObject::camera = camera; }
	static void SetLightCamera(LightCamera* lightCamera) { InstanceObject::lightCamera = lightCamera; }


protected: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C���Z�b�g
	static std::array<PipelineSet, int(PipelineType::Size)> pipelineSet;
	//�p�C�v���C���Z�b�g
	static PipelineSet pipelineSetLightView;
	//���C�g
	static LightGroup* lightGroup;
	//�J����
	static Camera* camera;
	//�e�p�����J����
	static LightCamera* lightCamera;

protected: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;
	ComPtr<ID3D12Resource> constBuffLightViewB0;
	ComPtr<ID3D12Resource> constBuffLightViewB1;
	//�A�t�B���ϊ����
	ConstBufferDataB1 objInform;
	//�C���X�^���V���O�`���
	int instanceDrawNum = 0;
	//���f��
	ObjModel* model = nullptr;
	//�J�����ɒǏ]���邩
	bool isCameraFollow = false;
	//�e��t���邩
	bool isShadowMap = false;
};
