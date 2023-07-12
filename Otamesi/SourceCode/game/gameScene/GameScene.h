﻿#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "GameCamera.h"
#include "LightCamera.h"
#include "Player.h"
#include "MapDataStage.h"
#include "UserInterface.h"
#include "BackGround.h"
#include "Skydome.h"
#include "ClearStaging.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene :public BaseGameScene
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	void DrawBackSprite() override;

	/// <summary>
	/// 3Dシーン描画
	/// </summary>
	void Draw3D() override;
	void AfterBloomDraw() override;

	/// <summary>
	/// 3Dシーン描画(ライトから見た視点)
	/// </summary>
	void Draw3DLightView() override;

	/// <summary>
	/// 前景スプライト描画
	/// </summary>
	void DrawFrontSprite() override;

	/// <summary>
	/// メニューの動き
	/// </summary>
	void MenuAction() override;

	/// <summary>
	/// 再生成
	/// </summary>
	void ReCreate();

	/// <summary>
	/// ゲームを開始状態に戻す
	/// </summary>
	void RestartGame();

	/// <summary>
	/// frameごとの初期化
	/// </summary>
	void FrameReset()override;

private: //メンバ変数
	//カメラ
	std::unique_ptr<GameCamera> camera;
	//影用光源カメラ
	std::unique_ptr<LightCamera> lightCamera;

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//objモデル
	std::unique_ptr<ObjModel> modelPlayer;
	std::unique_ptr<ObjModel> modelPlayerEffect;
	std::unique_ptr<ObjModel> modelSkydome;

	//プレイヤー
	std::unique_ptr<Player> player;
	//ステージ用マップデータ
	std::unique_ptr<MapDataStage> mapData;
	//天球
	std::unique_ptr<Skydome> skydome;

	//UI関係
	std::unique_ptr<UserInterface> userInterface_;
	//ステージクリアUI
	std::unique_ptr<ClearStaging> stageClear_;

	//背景オブジェクト
	std::unique_ptr<BackGround> backGround;

	// パラノマスカイドーム
	//std::unique_ptr<ParanomaSkyDorm> paranomaSkyDorm;

	//ステージクリアフラグ
	bool isStageClear = false;
};