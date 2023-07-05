#pragma once

#include <DirectXMath.h>
#include "Texture.h"
#include "PipelineSet.h"

class OutLineDraw
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



public: //�ÓI�����o�֐�
	/// <summary>
	/// �V���h�E�}�b�v����
	/// </summary>
	/// <returns>�V���h�E�}�b�v</returns>
	static OutLineDraw *Create();

	/// <summary>
	/// �V���h�E�}�b�v���ʕ����̏�����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void OutLineCommon(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList)
	{
		//nullptr�`�F�b�N
		assert(dev);
		assert(cmdList);

		OutLineDraw::dev = dev;
		OutLineDraw::cmdList = cmdList;
	};

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	void DrawScenePrev(ID3D12DescriptorHeap *descHeapDSV);

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	void DrawSceneRear();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipelineState();

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device *dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList *cmdList;
	static PipelineSet pipelineSet;

private: //�����o�ϐ�

	void DrawAfterScenePrev(ID3D12DescriptorHeap *descHeapDSV);

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	//ComPtr<ID3D12Resource> constBuff;
	//�e�N�X�`��
	Texture texture;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

};