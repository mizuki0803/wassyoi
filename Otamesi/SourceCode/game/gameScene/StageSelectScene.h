#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "ObjObject3d.h"
#include "Camera.h"
#include "MapDataStageSelectManager.h"
#include "UserInterface.h"

/// <summary>
/// ステージ選択シーン
/// </summary>
class StageSelectScene :public BaseGameScene
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

private: //メンバ変数
	//カメラ
	std::unique_ptr<Camera> camera;
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

	//マップデータ
	std::unique_ptr<MapDataStageSelectManager> mapDataManager;

	//objモデル
	std::unique_ptr<ObjModel> modelSkydome;

	//天球
	std::unique_ptr<ObjObject3d> skydome;

	//UI関係
	std::unique_ptr<UserInterface> userInterface_;
};