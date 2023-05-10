#pragma once
#include <DirectXMath.h>
#include "PipelineSet.h"
#include "Texture.h"

/// <summary>
/// ポストエフェクト
/// </summary>
class PostEffect
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

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		unsigned int isRadialBlur; //ラジアルブラーをかけるか
		unsigned int radialBlurSampleNum; //ラジアルブラーのサンプル回数
		float radialBlurStrength; //ラジアルブラーの広がる強さ
		float pad1;
	};

public: //定数
	//ラジアルブラーの最小サンプル回数
	static const int radialBlurSampleNumMin = 1;

public: //静的メンバ関数
	/// <summary>
	/// ポストエフェクト生成
	/// </summary>
	/// <returns>ポストエフェクト</returns>
	static PostEffect* Create();

	/// <summary>
	/// ポストエフェクト共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void PostEffectCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

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
	void DrawScenePrev();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void DrawSceneRear();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState();

	//getter
	const bool GetIsRadialBlur() { return isRadialBlur; }
	const int GetRadialBlurSampleNum() { return radialBlurSampleNum; }
	const float GetRadialBlurStrength() { return radialBlurStrength; }

	//setter
	void SetRadialBlur(const bool isRadialBlur) { this->isRadialBlur = isRadialBlur; }
	void SetRadialBlurSampleNum(const int radialBlurSampleNum) { this->radialBlurSampleNum = radialBlurSampleNum; }
	void SetRadialBlurStrength(const float radialBlurStrength) { this->radialBlurStrength = radialBlurStrength; }

private: //静的メンバ変数
	//画面クリアカラー
	static const float clearColor[4];
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプラインセット
	static PipelineSet pipelineSet;

private: //メンバ変数
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//テクスチャ
	Texture texture;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//色(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//ラジアルブラーをかけるか
	bool isRadialBlur = false;
	//ラジアルブラーのサンプル回数
	int radialBlurSampleNum = 10;
	//ラジアルブラーの広がる強さ
	float radialBlurStrength = 0.5f;
};

