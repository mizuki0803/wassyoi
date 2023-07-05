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

/// <summary>
/// obj3D�I�u�W�F�N�g
/// </summary>
class ObjObject3d
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
		XMFLOAT4 color;		//�F
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		Vector3 cameraPos;	//�J�������W(���[���h���W)
		XMMATRIX lightViewproj;	//���C�g�r���[�v���W�F�N�V�����s��
		XMMATRIX topLightViewproj;	//���ド�C�g�r���[�v���W�F�N�V�����s��
		unsigned int isShadowMap;	//�e��t���邩
	};
	//�e�p
	struct ConstBufferDataLightViewB0
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		Vector3 cameraPos;	//�J�������W(���[���h���W)
	};


public: //�ÓI�����o�֐�
	/// <summary>
	/// Object3d���ʕ����̏�����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void Object3dCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreatePipeline();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateLightViewPipeline();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateAddPipeline();

	/// <summary>
	/// �`��O����
	/// </summary>
	static void DrawPrev();

	/// <summary>
	/// �`��O����
	/// </summary>
	static void DrawLightViewPrev();

	/// <summary>
	/// �`��O����
	/// </summary>
	static void DrawAddPrev();

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>object3d</returns>
	static ObjObject3d* Create(ObjModel* model);


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
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	virtual void DrawLightCameraView();

	//getter
	const Vector3 GetWorldPos();
	const Vector3& GetPosition() { return position; }
	const Vector3& GetRotation() { return rotation; }
	const Vector3& GetScale() { return scale; }
	const XMFLOAT4& GetColor() { return color; }
	const XMMATRIX& GetMatWorld() { return matWorld; };

	//setter
	void SetPosition(const Vector3& position) { this->position = position; }
	void SetRotation(const Vector3& rotation) { this->rotation = rotation; }
	void SetScale(const Vector3& scale) { this->scale = scale; }
	void SetColor(const XMFLOAT4& color) { this->color = color; }
	void SetModel(ObjModel* model) { this->model = model; }
	void SetIsCameraFollow(bool isCameraFollow) { this->isCameraFollow = isCameraFollow; }
	void SetIsShadowMap(bool isShadowMap) { this->isShadowMap = isShadowMap; }
	static void SetLightGroup(LightGroup* lightGroup) { ObjObject3d::lightGroup = lightGroup; }
	static void SetCamera(Camera* camera) { ObjObject3d::camera = camera; }
	static void SetLightCamera(LightCamera* lightCamera) { ObjObject3d::lightCamera = lightCamera; }


protected: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C���Z�b�g
	static PipelineSet pipelineSet;
	//�p�C�v���C���Z�b�g
	static PipelineSet pipelineSetLightView;
	//�p�C�v���C���Z�b�g
	static PipelineSet pipelineSetAdd;
	//���C�g
	static LightGroup* lightGroup;
	//�J����
	static Camera* camera;
	//�e�p�����J����
	static LightCamera* lightCamera;


protected: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffLightViewB0;
	//�A�t�B���ϊ����
	Vector3 scale = { 1, 1, 1 };
	Vector3 rotation = { 0, 0, 0 };
	Vector3 position = { 0, 0, 0 };
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld = {};
	//�e�q�\��
	ObjObject3d* parent = nullptr;
	//���f��
	ObjModel* model = nullptr;
	//�J�����ɒǏ]���邩
	bool isCameraFollow = false;
	//�e��t���邩
	bool isShadowMap = false;
};