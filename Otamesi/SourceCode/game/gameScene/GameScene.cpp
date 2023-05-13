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
#include "JsonLoader.h"
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


	//jsonマップデータ読み込み
	LoadMapData("1-1");

	//カメラ初期化
	camera.reset(new GameCamera());
	const float distanceStageCenter = 60.0f; //カメラ視点のステージ中央からの距離
	const Vector3 stageCenterPos = { Block::GetBlockSize() / 2 * (mapSize.x - 1), Block::GetBlockSize() / 2 * (mapSize.y - 1), Block::GetBlockSize() / 2 * (mapSize.z - 1) }; //ステージ中央座標
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
					const Vector3 pos = { i * Block::GetBlockSize(), j * Block::GetBlockSize(), k * Block::GetBlockSize() };
					newBlock.reset(Block::Create(modelGoal.get(), pos));
					blocks.push_back(std::move(newBlock));
				}
				//ブロック生成
				else if (mapChipNum[i][j][k] == MapBlockData::BlockType::Block) {
					std::unique_ptr<Block> newBlock;
					const Vector3 pos = { i * Block::GetBlockSize(), j * Block::GetBlockSize(), k * Block::GetBlockSize() };
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

	BlockUpdate();

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

bool GameScene::LoadMapData(const std::string& fileName)
{
	//マップデータ情報読み取り
	std::vector<std::vector<std::vector<int>>> inputmap;
	float _cameraDist;	//これは今関係ない

	if (!JsonLoader::DeserializeTest("Resources/mapdata/" + fileName + ".json", &_cameraDist, &inputmap)) {
		return false;
	}

	//マップのサイズを取得
	const int marginNum = 2; //ゲームの仕様上、上下左右前後に1マスの余白を作る
	mapSize = { int(inputmap[0][0].size()) + marginNum, int(inputmap.size()) + marginNum, int(inputmap[0].size()) + marginNum };

	//余白ありの大きさでマップデータを生成。まずは0で埋める
	mapChipNum.resize(mapSize.x);
	for (int x = 0; x < mapSize.x; x++) {
		mapChipNum[x].resize(mapSize.y);
		for (int y = 0; y < mapSize.y; y++) {
			mapChipNum[x][y].resize(mapSize.z);
		}
	}

	//余白ではない部分の要素番号に読み込んだマップデータ情報の番号を割り当てていく
	for (int x = 0; x < mapSize.x - marginNum; x++) {
		for (int y = 0; y < mapSize.y - marginNum; y++) {
			for (int z = 0; z < mapSize.z - marginNum; z++) {
				mapChipNum[x + 1][y + 1][z + 1] = inputmap[y][z][x];
			}
		}
	}

	return true;
}

void GameScene::BlockUpdate()
{
	if (!isBolckUp_)
	{
		// 最大値の数
		int timeCount = 0;
		// 上げた個数
		int blockUpCount = 0;
		// 何個ずつ上げるかの計算
		int UpBlockSize = static_cast<int>(blocks.size() / 3.0f);
		blockActTimer_++;
		//マップ用ブロック
		for (const std::unique_ptr<Block>& block : blocks) {
			if (blockActTimer_ <= 10 * timeCount)
			{
				break;
			}
			block->Update();

			if (blockUpCount >= UpBlockSize)
			{
				timeCount++;
				blockUpCount = 0;
			}
			blockUpCount++;
		}

		int count = static_cast<int>(blocks.size() - 1);
		if (blocks[count]->GetActPhase() == 1)
		{
			isBolckUp_ = true;
		}
	}
	else
	{
		//マップ用ブロック
		for (const std::unique_ptr<Block>& block : blocks) {
			block->Update();
		}
	}
}
