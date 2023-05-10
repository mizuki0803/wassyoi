#pragma once
#include "PipelineSet.h"
#include "Texture.h"
#include <DirectXMath.h>
#include <string>

/// <summary>
/// スプライト共通部分
/// </summary>
class SpriteCommon final
{
private: //エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //シングルトン化
	//コンストラクタを隠蔽
	SpriteCommon() = default;
	//デストラクタを隠蔽
	~SpriteCommon() = default;
public:
	//コピーコンストラクタを無効化
	SpriteCommon(const SpriteCommon& spriteCommon) = delete;
	//代入演算子を無効化
	void operator = (const SpriteCommon& spriteCommon) = delete;

public: //メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>スプライト共通部分</returns>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="window_width">ウインドウ横幅</param>
	/// <param name="window_height">ウインドウ縦幅</param>
	/// <param name="directoryPath">ディレクトリパス</param>
	void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	/// <summary>
	/// 描画前処理	
	/// </summary>
	void DrawPrev();

private: //メンバ関数
	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreatePipeline();

public: //getter
	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return dev; }

	/// <summary>
	/// 射影行列取得
	/// </summary>
	/// <returns>射影行列</returns>
	const XMMATRIX& GetMatProjection() { return matProjection; }

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList; }

	/// <summary>
	/// パイプラインステート取得
	/// </summary>
	/// <returns>パイプラインステート</returns>
	const PipelineSet& GetPipelineSet() { return pipelineSet; }

private: //メンバ変数
	//デバイス
	ID3D12Device* dev = nullptr;
	//コマンドリスト
	ID3D12GraphicsCommandList* cmdList = nullptr;
	//パイプラインセット
	PipelineSet pipelineSet;
	//射影行列
	XMMATRIX matProjection;
};