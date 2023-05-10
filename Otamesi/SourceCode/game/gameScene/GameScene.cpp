#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Collision.h"
#include "Easing.h"
#include "PlayerActionManager.h"
#include "MapBlockData.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void GameScene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	//objからモデルデータを読み込む
	modelPlayer.reset(ObjModel::LoadFromOBJ("player"));
	modelBlock.reset(ObjModel::LoadFromOBJ("block"));
	modelGoal.reset(ObjModel::LoadFromOBJ("goal"));
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01", true));

	//仮マップ番号
	/*const int mapSize = 7;
	int mapData[mapSize][mapSize][mapSize] = {

		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,1,0,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,3,3,3,3,3,0},
			{0,0,3,3,3,0,0},
			{0,0,3,0,3,0,0},
			{0,0,3,3,3,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,3,3,2,3,3,0},
			{0,0,3,3,3,2,0},
			{0,0,0,3,0,0,0},
			{0,0,3,3,3,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,3,3,3,3,3,0},
			{0,0,3,3,3,0,0},
			{0,0,3,0,3,0,0},
			{0,0,3,3,3,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
	};*/

	const int mapSizeYZ = 7;
	const int mapSizeX = 8;
	int mapData[mapSizeX][mapSizeYZ][mapSizeYZ] = {
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,1,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,3,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,3,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,2,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,3,0,0,0,0,0},
			{0,3,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,3,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,3,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
	};

	/*const int mapSize = 3;
	int mapData[mapSize][mapSize][mapSize] = {
		{
			{1,2,3},
			{3,3,3},
			{3,3,3},
		},
		{
			{3,3,3},
			{3,3,3},
			{3,3,3},
		},
		{
			{3,3,3},
			{3,3,3},
			{3,3,3},
		}
	};*/

	//マップ生成
	mapChipNum.resize(mapSizeX);
	for (int i = 0; i < mapSizeX; i++) {
		mapChipNum[i].resize(mapSizeYZ);
		for (int j = 0; j < mapSizeYZ; j++) {
			for (int k = 0; k < mapSizeYZ; k++) {
				mapChipNum[i][j].push_back(mapData[i][j][k]);
			}
		}
	}

	//カメラ初期化
	camera.reset(new GameCamera());
	const float distanceStageCenter = 60.0f; //カメラ視点のステージ中央からの距離
	const Vector3 stageCenterPos = { Block::GetBlockSize() / 2 * (mapSizeX - 1), -Block::GetBlockSize() / 2 * (mapSizeYZ - 1), Block::GetBlockSize() / 2 * (mapSizeYZ - 1) }; //ステージ中央座標
	camera->Initialize(distanceStageCenter, stageCenterPos);
	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });

	//マップ用ブロック生成
	for (int i = 0; i < mapChipNum.size(); i++) {
		for (int j = 0; j < mapChipNum[i].size(); j++) {
			for (int k = 0; k < mapChipNum[i][j].size(); k++) {
				//プレイヤー生成
				if (mapChipNum[i][j][k] == MapBlockData::BlockType::Player) {
					player.reset(Player::Create(modelPlayer.get(), { i, j, k }, camera.get()));
				}
				//ゴール生成
				else if (mapChipNum[i][j][k] == MapBlockData::BlockType::Goal) {
					std::unique_ptr<Block> newBlock;
					const Vector3 pos = { k * Block::GetBlockSize(), -j * Block::GetBlockSize(), i * Block::GetBlockSize() };
					newBlock.reset(Block::Create(modelGoal.get(), pos));
					blocks.push_back(std::move(newBlock));
				}
				//ブロック生成
				else if (mapChipNum[i][j][k] == MapBlockData::BlockType::Block) {
					std::unique_ptr<Block> newBlock;
					const Vector3 pos = { k * Block::GetBlockSize(), -j * Block::GetBlockSize(), i * Block::GetBlockSize() };
					newBlock.reset(Block::Create(modelBlock.get(), pos));
					blocks.push_back(std::move(newBlock));
				}
			}
		}
	}
	//プレイヤーの移動可能判定用にマップ番号をセット
	PlayerActionManager::SetMapChipNum(mapChipNum);

	//天球生成
	skydome.reset(ObjObject3d::Create(modelSkydome.get()));
	skydome->SetPosition({});
	skydome->SetScale({ 2, 2, 2 });

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(camera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());

	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());
}

void GameScene::Update()
{
	//デバッグ用テキスト
	DebugText::GetInstance()->Print("CameraMove : arrow", 10, 10);
	DebugText::GetInstance()->Print("PlayerMove : WASD", 10, 30);

	//プレイヤーがゴールをしたらステージクリア
	if (!isStageClear) {
		if (player->GetIsGoal()) { isStageClear = true; }
	}
	else {
		DebugText::GetInstance()->Print("STAGECLEAR", 100, 300, 5);
	}

	//カメラ更新
	camera->Update();
	lightCamera->Update();

	//ライト更新
	lightGroup->Update();

	//オブジェクト更新
	//プレイヤー
	player->Update();
	//マップ用ブロック
	for (const std::unique_ptr<Block>& block : blocks) {
		block->Update();
	}
	//天球
	skydome->Update();

	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void GameScene::DrawBackSprite()
{
}

void GameScene::Draw3D()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///

	//プレイヤー
	player->Draw();
	//マップ用ブロック
	for (const std::unique_ptr<Block>& block : blocks) {
		block->Draw();
	}
	//天球
	skydome->Draw();

	///-------Object3d描画ここまで-------///
}

void GameScene::Draw3DLightView()
{
}

void GameScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///


	///-------スプライト描画ここまで-------///
}