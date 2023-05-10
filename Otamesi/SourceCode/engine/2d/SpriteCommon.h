#pragma once
#include "PipelineSet.h"
#include "Texture.h"
#include <DirectXMath.h>
#include <string>

/// <summary>
/// �X�v���C�g���ʕ���
/// </summary>
class SpriteCommon final
{
private: //�G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�V���O���g����
	//�R���X�g���N�^���B��
	SpriteCommon() = default;
	//�f�X�g���N�^���B��
	~SpriteCommon() = default;
public:
	//�R�s�[�R���X�g���N�^�𖳌���
	SpriteCommon(const SpriteCommon& spriteCommon) = delete;
	//������Z�q�𖳌���
	void operator = (const SpriteCommon& spriteCommon) = delete;

public: //�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�X�v���C�g���ʕ���</returns>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="window_width">�E�C���h�E����</param>
	/// <param name="window_height">�E�C���h�E�c��</param>
	/// <param name="directoryPath">�f�B���N�g���p�X</param>
	void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	/// <summary>
	/// �`��O����	
	/// </summary>
	void DrawPrev();

private: //�����o�֐�
	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreatePipeline();

public: //getter
	/// <summary>
	/// �f�o�C�X�擾
	/// </summary>
	/// <returns>�f�o�C�X</returns>
	ID3D12Device* GetDevice() { return dev; }

	/// <summary>
	/// �ˉe�s��擾
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	const XMMATRIX& GetMatProjection() { return matProjection; }

	/// <summary>
	/// �R�}���h���X�g�擾
	/// </summary>
	/// <returns>�R�}���h���X�g</returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList; }

	/// <summary>
	/// �p�C�v���C���X�e�[�g�擾
	/// </summary>
	/// <returns>�p�C�v���C���X�e�[�g</returns>
	const PipelineSet& GetPipelineSet() { return pipelineSet; }

private: //�����o�ϐ�
	//�f�o�C�X
	ID3D12Device* dev = nullptr;
	//�R�}���h���X�g
	ID3D12GraphicsCommandList* cmdList = nullptr;
	//�p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	//�ˉe�s��
	XMMATRIX matProjection;
};