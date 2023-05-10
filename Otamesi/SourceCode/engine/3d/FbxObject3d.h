#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3d
{
protected: //�G�C���A�X
	//MIcrosoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��p)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		Vector3 cameraPos;	//�J�������W(���[���h���W)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin 
	{
		XMMATRIX bones[MAX_BONES];
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCommandList(ID3D12GraphicsCommandList* cmdList) { FbxObject3d::cmdList = cmdList; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="fbxModel">FBX���f��</param>
	void SetFbxModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

	//getter
	const Vector3& GetPosition() { return position; }
	const Vector3& GetRotation() { return rotation; }
	const Vector3& GetScale() { return scale; }

	//setter
	void SetPosition(const Vector3& position) { this->position = position; }
	void SetRotation(const Vector3& rotation) { this->rotation = rotation; }
	void SetScale(const Vector3& scale) { this->scale = scale; }
	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�J����
	static Camera* camera;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

protected: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBufferTransform;
	//���[�J���X�P�[��
	Vector3 scale = { 1, 1, 1 };
	//X,Y,Z�����̃��[�J����]�p
	Vector3 rotation = { 0, 0, 0 };
	//���[�J�����W
	Vector3 position = { 0, 0, 0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//FBX���f��
	FbxModel* fbxModel = nullptr;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBufferSkin;
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
};