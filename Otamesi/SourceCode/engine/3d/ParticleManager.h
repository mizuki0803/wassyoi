#pragma once
#include <Windows.h>
#include "DirectXBase.h"
#include <DirectXMath.h>
#include <forward_list>
#include <functional>
#include "Camera.h"
#include "PipelineSet.h"
#include "Texture.h"

/// <summary>
/// パーティクルマネージャー
/// </summary>
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//頂点データ構造体
	struct VertexPos
	{
		Vector3 pos;	//xyz座標
		float scale;	//スケール
		float rot;	//回転角
		XMFLOAT4 color;	//色
	};

	//定数バッファ用データ構造体B0
	struct ConstBufferData
	{
		XMMATRIX mat;	//ビュープロジェクション行列
		XMMATRIX matBillboad;	//ビルボード行列
	};

	//パーティクル1粒
	struct Particle
	{
		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};
		//追従用対象座標取得関数
		std::function<Vector3()> getTargetPos;
		//追従用ローカル座標
		Vector3 localPos = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int numFrame = 0;
		//スケール
		float scale = 1.0f;
		//初期値
		float startScale = 1.0f;
		//最終値
		float endScale = 0.0f;
		//スケール変更速さ
		std::function<float(const float, const float, const float)> easingScale;
		//回転角
		float rotation = 0;
		//回転速度
		float rotSpeed = 0;
		//色
		XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//色初期値
		XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		//色終了値
		XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

public:
	/// <summary>
	/// パーティクル共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void ParticleManagerCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 加算合成パイプライン生成
	/// </summary>
	static void CreateAddBlendPipeline();

	/// <summary>
	/// 減算合成パイプライン生成
	/// </summary>
	static void CreateSubBlendPipeline();

	/// <summary>
	/// 加算合成描画前処理
	/// </summary>
	static void DrawPrevAddBlend();

	/// <summary>
	/// 減算合成描画前処理
	/// </summary>
	static void DrawPrevSubBlend();

	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="texture">テクスチャ</param>
	/// <returns>パーティクルマネージャー</returns>
	static ParticleManager* Create(const Texture& texture);

	/// <summary>
	/// パーティクルの情報をセット
	/// </summary>
	/// <param name="life">寿命</param>
	/// <param name="position">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="startScale">スケール初期値</param>
	/// <param name="endScale">スケール終了値</param>
	/// <param name="easingScale">イージングスケール変更</param>
	/// <param name="startColor">色初期値</param>
	/// <param name="endColor">色終了値</param>
	/// <param name="rotation">角度</param>
	/// <param name="rotSpeed">回転速度</param>
	void Add(const int life, const Vector3& position, const Vector3& velocity, const Vector3& accel,
		const float startScale, const float endScale, std::function<float(const float, const float, const float)> easingScale,
		const XMFLOAT4& startColor, const XMFLOAT4& endColor, const float rotation = 0, const float rotSpeed = 0);

	/// <summary>
	/// 対象座標を追従するパーティクルの情報をセット
	/// </summary>
	/// <param name="life">寿命</param>
	/// <param name="getTargetPos">追従用対象座標取得関数</param>
	/// <param name="localPos">ローカル座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="startScale">スケール初期値</param>
	/// <param name="endScale">スケール終了値</param>
	/// <param name="easingScale">イージングスケール変更</param>
	/// <param name="startColor">色初期値</param>
	/// <param name="endColor">色終了値</param>
	/// <param name="rotation">角度</param>
	/// <param name="rotSpeed">回転速度</param>
	void AddTargetFollow(const int life, std::function<Vector3()> getTargetPos, const Vector3& localPos, const Vector3& velocity, const Vector3& accel,
		const float startScale, const float endScale, std::function<float(const float, const float, const float)> easingScale,
		const XMFLOAT4& startColor, const XMFLOAT4& endColor, const float rotation = 0, const float rotSpeed = 0);

	/// <summary>
	/// パーティクルの形生成
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	bool CreateModel(const Texture& texture);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 全削除
	/// </summary>
	void AllDelete();

	//setter
	static void SetCamera(Camera* camera) { ParticleManager::camera = camera; }

private:
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//加算合成パイプラインセット
	static PipelineSet addBlendPipelineSet;
	//減算合成パイプラインセット
	static PipelineSet subBlendPipelineSet;
	//頂点数
	static const int vertexCount = 2048;
	//頂点データ配列
	VertexPos vertices[vertexCount];
	//カメラ
	static Camera* camera;

private:
	//頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//テクスチャ
	Texture texture;
	//パーティクル配列
	std::forward_list<Particle> particles;
};