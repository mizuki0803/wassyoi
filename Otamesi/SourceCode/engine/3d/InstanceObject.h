#pragma once
#include <Windows.h>
#include "DirectXBase.h"
#include <DirectXMath.h>
#include "PipelineSet.h"
#include "ObjModel.h"
#include "LightGroup.h"
#include "Camera.h"
#include "LightCamera.h"
#include "Vector3.h"
#include "ObjObject3d.h"
#include <array>

/// <summary>
/// インスタンスオブジェクト
/// </summary>
class InstanceObject
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	//定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;	//ビュープロジェクション行列
		Vector3 cameraPos;	//カメラ座標(ワールド座標)
		XMMATRIX lightViewproj;	//ライトビュープロジェクション行列
		unsigned int isShadowMap;	//影を付けるか
	};

	//定数バッファ用データ構造体B0
	static const int draw_max_num = 512;
	struct ConstBufferDataB1
	{
		XMFLOAT4 color[draw_max_num];		//色
		XMMATRIX world[draw_max_num];		//ワールド行列
	};

	//影用
	struct ConstBufferDataLightViewB0
	{
		XMMATRIX viewproj;	//ビュープロジェクション行列
		Vector3 cameraPos;	//カメラ座標(ワールド座標)
	};

	struct ConstBufferDataLightViewB1
	{
		XMMATRIX world[draw_max_num];		//ワールド行列
	};

	enum class PipelineType
	{
		Alpha,
		Add,
		OutLine,
		Size,
	};

public: //静的メンバ関数
	/// <summary>
	/// Object3d共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void InstanceObjectCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateAlphaPipeline();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateAddPipeline();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateLightViewPipeline();

	static void CreateOutLinePipeline();

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void DrawPrev(PipelineType _type = PipelineType::Alpha);

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void DrawLightViewPrev();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>object3d</returns>
	static InstanceObject* Create(ObjModel* model);


public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	virtual bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画セット
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_scale">大きさ</param>
	/// <param name="_rotation">回転角</param>
	/// <param name="_color">色</param>
	/// <param name="_parentWorld">親オブジェクトワールド行列</param>
	void DrawInstance(const XMFLOAT3& _pos, const XMFLOAT3& _scale,
		const XMFLOAT3& _rotation, const XMFLOAT4& _color, const XMMATRIX* _parentWorld = nullptr);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 影用光源ライトから見た視点での描画
	/// </summary>
	virtual void DrawLightCameraView();

	/// <summary>
	/// インスタンシング描画最大描画数になっていないかのチェック
	/// </summary>
	/// <returns></returns>
	bool GetInstanceDrawCheck() { return instanceDrawNum < draw_max_num; }

	/// <summary>
	/// インスタンシング描画個数
	/// </summary>
	/// <returns></returns>
	int GetInstanceDrawNum() { return instanceDrawNum; }

	/// <summary>
	/// frameごとの初期化
	/// </summary>
	void FrameReset();

	//setter
	void SetModel(ObjModel* model) { this->model = model; }
	void SetIsCameraFollow(bool isCameraFollow) { this->isCameraFollow = isCameraFollow; }
	void SetIsShadowMap(bool isShadowMap) { this->isShadowMap = isShadowMap; }
	static void SetLightGroup(LightGroup* lightGroup) { InstanceObject::lightGroup = lightGroup; }
	static void SetCamera(Camera* camera) { InstanceObject::camera = camera; }
	static void SetLightCamera(LightCamera* lightCamera) { InstanceObject::lightCamera = lightCamera; }


protected: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプラインセット
	static std::array<PipelineSet, int(PipelineType::Size)> pipelineSet;
	//パイプラインセット
	static PipelineSet pipelineSetLightView;
	//ライト
	static LightGroup* lightGroup;
	//カメラ
	static Camera* camera;
	//影用光源カメラ
	static LightCamera* lightCamera;

protected: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;
	ComPtr<ID3D12Resource> constBuffLightViewB0;
	ComPtr<ID3D12Resource> constBuffLightViewB1;
	//アフィン変換情報
	ConstBufferDataB1 objInform;
	//インスタンシング描画個数
	int instanceDrawNum = 0;
	//モデル
	ObjModel* model = nullptr;
	//カメラに追従するか
	bool isCameraFollow = false;
	//影を付けるか
	bool isShadowMap = false;
};
