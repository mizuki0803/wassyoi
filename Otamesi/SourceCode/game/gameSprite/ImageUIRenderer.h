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
	// テクスチャ
	enum class TexName
	{
		Move,
		Camera,
		Max
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


	Texture &GetTex(TexName name)
	{
		return texture_[static_cast<int>(name)];
	}
private:

	// 初期化
	bool Initialize();

	//デバイス
	static ID3D12Device *dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList *cmdList;

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


	Texture texture_[static_cast<int>(TexName::Max)];



	// 
	enum class KeyObjectName
	{
		W,A,S,D,
		Up,Left,Down,Right,
		Max
	};
	// キーオブジェクト
	std::unique_ptr<ObjObject3d> key_[static_cast<int>(KeyObjectName::Max)];
	// プレイヤー
	std::unique_ptr<ObjObject3d> move_player_;
	// プレイヤー座標
	float player_pos_[6]
	{
		-2.5f,
		-1.5f,
		-0.5f,
		0.5f,
		1.5f,
		2.5f
	};

	unsigned int moveFrameCounter_{ 0 };
	unsigned int cameraFrameCounter_{ 0 };
	// 移動説明用カメラ
	std::unique_ptr<Camera> player_camera_;

	// 
	enum class ArrowObjectName
	{
		Left, Right,
		SpinLeft, SpinRight,
		Max
	};


	// カメラオブジェクト
	std::unique_ptr<ObjObject3d> camera_;
	// カメラ用Player
	std::unique_ptr<ObjObject3d> camera_player_;
	// アロー
	std::unique_ptr<ObjObject3d> arrow_[static_cast<int>(ArrowObjectName::Max)];
	// カメラ説明用カメラ
	std::unique_ptr<Camera> camera_camera_;

	float camera_rot_[3]
	{
		90.0f,
		0.0f,
		-90.0f
	};

};

