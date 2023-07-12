#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3d
{
protected: //エイリアス
	//MIcrosoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//定数
	//ボーンの最大数
	static const int MAX_BONES = 32;

public: //サブクラス
	//定数バッファ用データ構造体(座標変換行列用)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//ビュープロジェクション行列
		XMMATRIX world;		//ワールド行列
		Vector3 cameraPos;	//カメラ座標(ワールド座標)
	};

	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin 
	{
		XMMATRIX bones[MAX_BONES];
	};

public: //静的メンバ関数
	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCommandList(ID3D12GraphicsCommandList* cmdList) { FbxObject3d::cmdList = cmdList; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="fbxModel">FBXモデル</param>
	void SetFbxModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

	//getter
	const Vector3& GetPosition() { return position; }
	const Vector3& GetRotation() { return rotation; }
	const Vector3& GetScale() { return scale; }

	//setter
	void SetPosition(const Vector3& position) { this->position = position; }
	void SetRotation(const Vector3& rotation) { this->rotation = rotation; }
	void SetScale(const Vector3& scale) { this->scale = scale; }
	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//カメラ
	static Camera* camera;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

protected: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBufferTransform;
	//ローカルスケール
	Vector3 scale = { 1, 1, 1 };
	//X,Y,Z軸回りのローカル回転角
	Vector3 rotation = { 0, 0, 0 };
	//ローカル座標
	Vector3 position = { 0, 0, 0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//FBXモデル
	FbxModel* fbxModel = nullptr;
	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBufferSkin;
	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション)
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
};