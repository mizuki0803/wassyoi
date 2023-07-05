#pragma once
#include "WindowApp.h"
#include "DirectXBase.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "ShadowMap.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "AfterBloom.h"
#include "OutLine.h"
#include "OutLineDraw.h"

/// <summary>
/// フレームワーク
/// </summary>
class FrameWork
{
public: //メンバ関数
	/// <summary>
	/// 実行
	/// </summary>
	void Run();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ゲームループ終了リクエストのgetter
	/// </summary>
	/// <returns>ゲームループ終了リクエスト</returns>
	virtual bool GetIsEndRequest() { return isEndRequest; }

protected: //メンバ変数
	//ゲームループ終了リクエスト
	bool isEndRequest = false;
	//ウィンドウアプリケーション
	std::unique_ptr<WindowApp> win;
	//DirectX基盤
	std::unique_ptr<DirectXBase> dxbase;
	//入力
	Input* input = nullptr;
	//音
	Audio* audio = nullptr;
	//スプライトの共通部分
	SpriteCommon* spriteCommon = nullptr;
	//デバッグテキスト
	DebugText* debugText = nullptr;
	//シャドウマップ(影生成ライト視点用)
	std::unique_ptr<ShadowMap> shadowMap;
	std::unique_ptr<AfterBloom> afterBloom;
	std::unique_ptr<OutLine> outLine;
	std::unique_ptr<OutLineDraw> outLineDraw;
	//シーン工場
	std::unique_ptr<AbstractSceneFactory> sceneFactory;
};