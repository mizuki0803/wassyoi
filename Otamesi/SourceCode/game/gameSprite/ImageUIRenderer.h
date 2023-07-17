#pragma once
#include "ObjObject3d.h"
#include "Texture.h"
#include "PipelineSet.h"


class ImageUIRenderer
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


	/// <summary>
	/// アウトライン生成
	/// </summary>
	/// <returns>シャドウマップ</returns>
	static ImageUIRenderer *Create();


	/// <summary>
	/// シャドウマップ共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void Common(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList)
	{
		//nullptrチェック
		assert(dev);
		assert(cmdList);

		ImageUIRenderer::dev = dev;
		ImageUIRenderer::cmdList = cmdList;
	};


	// 更新用
	void Update(bool isMoveMenu, bool isCameraMenu);



	// カメラ説明用画像書き出し
	void DrawCameraDescription();

	// 移動説明用画像書き出し
	void DrawMoveDescription();


private:

	// 初期化
	bool Initialize();

	//デバイス
	static ID3D12Device *dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList *cmdList;
	//パイプラインセット
	static PipelineSet pipelineSet;

	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	//深度バッファ用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;


	// テクスチャ
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
	// キーオブジェクト
	std::unique_ptr<ObjObject3d> key_[static_cast<int>(KeyObjectName::Max)];
	// プレイヤー
	std::unique_ptr<ObjObject3d> player_;


	// 
	enum class ArrowObjectName
	{
		Up, Left, Down, Right,
		Max
	};
	// カメラ
	std::unique_ptr<ObjObject3d> camera_;
	// アロー
	std::unique_ptr<ObjObject3d> arrow_[static_cast<int>(ArrowObjectName::Max)];
};

