#pragma once
#include "ObjObject3d.h"
#include "Texture.h"
#include "PipelineSet.h"


class ImageUIRenderer
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


	/// <summary>
	/// �A�E�g���C������
	/// </summary>
	/// <returns>�V���h�E�}�b�v</returns>
	static ImageUIRenderer *Create();


	/// <summary>
	/// �V���h�E�}�b�v���ʕ����̏�����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void Common(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList)
	{
		//nullptr�`�F�b�N
		assert(dev);
		assert(cmdList);

		ImageUIRenderer::dev = dev;
		ImageUIRenderer::cmdList = cmdList;
	};


	// �X�V�p
	void Update(bool isMoveMenu, bool isCameraMenu);



	// �J���������p�摜�����o��
	void DrawCameraDescription();

	// �ړ������p�摜�����o��
	void DrawMoveDescription();


private:

	// ������
	bool Initialize();

	//�f�o�C�X
	static ID3D12Device *dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList *cmdList;
	//�p�C�v���C���Z�b�g
	static PipelineSet pipelineSet;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	//�[�x�o�b�t�@�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;


	// �e�N�X�`��
	enum class TexName
	{
		Move,
		Camera,
		Max
	};
	Texture texture_[static_cast<int>(TexName::Max)];



	// 
	enum class KeyObjectName
	{
		W,A,S,D,
		Max
	};
	// �L�[�I�u�W�F�N�g
	std::unique_ptr<ObjObject3d> key_[static_cast<int>(KeyObjectName::Max)];
	// �v���C���[
	std::unique_ptr<ObjObject3d> player_;


	// 
	enum class ArrowObjectName
	{
		Up, Left, Down, Right,
		Max
	};
	// �J����
	std::unique_ptr<ObjObject3d> camera_;
	// �A���[
	std::unique_ptr<ObjObject3d> arrow_[static_cast<int>(ArrowObjectName::Max)];
};

