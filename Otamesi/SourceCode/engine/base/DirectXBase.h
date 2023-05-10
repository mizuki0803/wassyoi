#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <string>
#include <wrl.h>
#include <d3dx12.h>

#include "WindowApp.h"
#include <vector>

/// <summary>
/// DirectX���
/// </summary>
class DirectXBase
{
public:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="win">�E�C���h�E�A�v���P�[�V����</param>
	void Initialize(WindowApp* win);

	/// <summary>
	/// �`��O�ݒ�
	/// </summary>
	void GraphicsCommandPrev();

	/// <summary>
	/// �`���ݒ�
	/// </summary>
	void GraphicsCommandRear();

	/// <summary>
	/// �[�x�o�b�t�@�N���A
	/// </summary>
	void ClearDepthBuffer();

	//getter
	ID3D12Device* GetDevice() { return dev.Get(); };
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); };

private:
	/// <summary>
	/// �f�o�C�X������
	/// </summary>
	void InitializeDevice();

	/// <summary>
	/// �R�}���h������
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// �X���b�v�`�F�[��������
	/// </summary>
	void InitializeSwapchain();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[������
	/// </summary>
	void InitializeRenderTargetView();

	/// <summary>
	/// �[�x�o�b�t�@������
	/// </summary>
	void InitializeDepthBuffer();

	/// <summary>
	/// �t�F���X������
	/// </summary>
	void InitializeFence();

private:
	//�f�o�C�X
	ComPtr<ID3D12Device> dev;
	//DXGI�t�@�N�g���[
	ComPtr<IDXGIFactory6> dxgiFactory;
	//�R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//�R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//�R�}���h�L���[
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//�X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> swapchain;
	//�����_�[�^�[�Q�b�g�r���[�q�[�v
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	//�o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuffer;
	//�[�x�o�b�t�@�r���[�q�[�v
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//�t�F���X
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	//WindowApp
	WindowApp* win = nullptr;
};
