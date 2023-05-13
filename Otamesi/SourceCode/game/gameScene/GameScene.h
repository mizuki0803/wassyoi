#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "GameCamera.h"
#include "LightCamera.h"
#include "Player.h"
#include "Block.h"

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

private: //メンバ関数
	/// <summary>
	/// マップデータ読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	bool LoadMapData(const std::string& fileName);

	/// <summary>
	/// 
	/// </summary>
	void BlockUpdate();

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
	std::unique_ptr<ObjModel> modelBlock;
	std::unique_ptr<ObjModel> modelGoal;
	std::unique_ptr<ObjModel> modelSkydome;

	//プレイヤー
	std::unique_ptr<Player> player;

	//マップのサイズ
	XMINT3 mapSize;
	//3次元マップ用の3次元配列
	std::vector<std::vector<std::vector<int>>> mapChipNum;
	//マップ用ブロック
	std::vector<std::unique_ptr<Block>> blocks;

	//天球
	std::unique_ptr<ObjObject3d> skydome;

	//ステージクリアフラグ
	bool isStageClear = false;

	// ブロックを上げるタイマー
	int blockActTimer_ = 0;
	// ブロックを上げ終わったか
	bool isBolckUp_ = false;
};