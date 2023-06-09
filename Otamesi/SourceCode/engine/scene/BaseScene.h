#pragma once
#include <DirectXBase.h>

class SceneManager;

/// <summary>
/// 基底シーン
/// </summary>
class BaseScene
{
public: //メンバ関数
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	virtual void DrawBackSprite() = 0;

	/// <summary>
	/// 3Dシーン描画
	/// </summary>
	virtual void Draw3D() = 0;

	/// <summary>
	/// 3Dシーン描画(ライトから見た視点)
	/// </summary>
	virtual void Draw3DLightView() = 0;

	/// <summary>
	/// 前景スプライト描画
	/// </summary>
	virtual void DrawFrontSprite() = 0;

	//getter
	bool GetIsEndRequest() { return isEndRequest; }

	//setter
	static void SetDirectXbase(DirectXBase* dxbase) { BaseScene::dxbase = dxbase; }

protected: //静的メンバ変数
	static DirectXBase* dxbase;

protected: //メンバ変数
	//ゲームループ終了リクエスト
	bool isEndRequest = false;
};

