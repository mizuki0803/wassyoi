﻿#pragma once

#include <DirectXMath.h>
#include "Texture.h"
#include "PipelineSet.h"
#include "camera/Camera.h"

class OutLine
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	struct VertexPosUv
	{
		XMFLOAT3 pos;	//xyz座標
		XMFLOAT2 uv;	//uv座標
	};



public: //静的メンバ関数
	/// <summary>
	/// アウトライン生成
	/// </summary>
	/// <returns>シャドウマップ</returns>
	static OutLine *Create();

	/// <summary>
	/// シャドウマップ共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void OutLineCommon(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList)
	{
		//nullptrチェック
		assert(dev);
		assert(cmdList);

		OutLine::dev = dev;
		OutLine::cmdList = cmdList;
	};

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	void DrawScenePrev(ID3D12DescriptorHeap *descHeapDSV);

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void DrawSceneRear();

	static void SetCmaera(Camera *camera)
	{
		camera_ = camera;
	}

private: //静的メンバ変数
	//デバイス
	static ID3D12Device *dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList *cmdList;
	//パイプラインセット
	
	static PipelineSet pipelineSet;

private: //メンバ変数
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ
	//ComPtr<ID3D12Resource> constBuff;
	//テクスチャ
	enum TextureType
	{
		kColor,
		kDepth,
		kTexMax
	};
	Texture texture[kTexMax];
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//深度バッファ用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	struct CameraBuffer
	{
		XMMATRIX view_projection; // ビュープロジェクション
		XMMATRIX inv_view_projection; // ビュープロジェクション逆行列
	};
	ComPtr<ID3D12Resource> camera_buffer_;

	static Camera *camera_;
};


