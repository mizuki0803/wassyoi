#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <string>
#include <wrl.h>
#include <d3dx12.h>

#include "WindowApp.h"
#include <vector>

/// <summary>
/// DirectX基盤
/// </summary>
class DirectXBase
{
public:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="win">ウインドウアプリケーション</param>
	void Initialize(WindowApp* win);

	/// <summary>
	/// 描画前設定
	/// </summary>
	void GraphicsCommandPrev();

	/// <summary>
	/// 描画後設定
	/// </summary>
	void GraphicsCommandRear();

	/// <summary>
	/// 深度バッファクリア
	/// </summary>
	void ClearDepthBuffer();

	//getter
	ID3D12Device* GetDevice() { return dev.Get(); };
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); };

private:
	/// <summary>
	/// デバイス初期化
	/// </summary>
	void InitializeDevice();

	/// <summary>
	/// コマンド初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// スワップチェーン初期化
	/// </summary>
	void InitializeSwapchain();

	/// <summary>
	/// レンダーターゲットビュー初期化
	/// </summary>
	void InitializeRenderTargetView();

	/// <summary>
	/// 深度バッファ初期化
	/// </summary>
	void InitializeDepthBuffer();

	/// <summary>
	/// フェンス初期化
	/// </summary>
	void InitializeFence();

private:
	//デバイス
	ComPtr<ID3D12Device> dev;
	//DXGIファクトリー
	ComPtr<IDXGIFactory6> dxgiFactory;
	//コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//コマンドキュー
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//スワップチェーン
	ComPtr<IDXGISwapChain4> swapchain;
	//レンダーターゲットビューヒープ
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuffer;
	//深度バッファビューヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//フェンス
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	//WindowApp
	WindowApp* win = nullptr;
};
