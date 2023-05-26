#include "TitleScene.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "PlayerActionManager.h"
#include "ParticleEmitter.h"
#include "MapBlockData.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include "StageManager.h"
#include "JsonLoader.h"

void TitleScene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	//objからモデルデータを読み込む
	modelPlayer.reset(ObjModel::LoadFromOBJ("player"));
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01", true));

	//選択中のステージ番号を0にセット
	StageManager::SetSelectStage(0);
	//マップ生成
	mapData.reset(MapDataStage::Create(StageManager::GetSelectStage()));

	//カメラ初期化
	camera.reset(new GameCamera());
	const float distanceStageCenter = 60.0f; //カメラ視点のステージ中央からの距離
	const XMINT3 mapSize = mapData->GetMapSize(); //マップの大きさ
	const Vector3 stageCenterPos = { Block::GetBlockSize() / 2 * (mapSize.x - 1), Block::GetBlockSize() / 2 * (mapSize.y - 1), Block::GetBlockSize() / 2 * (mapSize.z - 1) }; //ステージ中央座標
	camera->Initialize(distanceStageCenter, stageCenterPos);
	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });

	//プレイヤー生成
	player.reset(Player::Create(modelPlayer.get(), mapData->GetPlayerCreateMapChipNum(), camera.get()));

	//プレイヤーの移動可能判定用にマップ番号をセット
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());

	//天球生成
	skydome.reset(ObjObject3d::Create(modelSkydome.get()));
	skydome->SetPosition({});
	skydome->SetScale({ 2, 2, 2 });

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(camera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());

	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());

	//objオブジェクトにカメラをセット
	InstanceObject::SetCamera(camera.get());
	InstanceObject::SetLightCamera(lightCamera.get());

	//objオブジェクトにライトをセット
	InstanceObject::SetLightGroup(lightGroup.get());

	//パーティクルにカメラをセット
	ParticleManager::SetCamera(camera.get());
	//画面にパーティクルが残ることがあるので全て削除しておく
	ParticleEmitter::GetInstance()->AllDelete();

	//初期状態をbinary保存
	Vector3 cameraPos = camera->GetEye();
	Vector3 playerPos = player->GetPosition();
	XMINT3 mapChip = player->GetMapChipNumberPos();

	JsonLoader::SerializeBinary(std::to_string(orderNum), camera->GetIs2D(), player->GetMoveSurfacePhase(),
		{ mapChip.x,mapChip.y,mapChip.z }, { cameraPos.x,cameraPos.y,cameraPos.z },
		{ camera->GetCameraXPosPhase(),camera->GetCameraYPosPhase() }, { playerPos.x, playerPos.y, playerPos.z });
}

void TitleScene::Finalize()
{
	DeleteBinary();
}

void TitleScene::Update()
{
	//デバッグ用テキスト
	DebugText::GetInstance()->Print("TITLE SCENE", 350, 200, 5);
	DebugText::GetInstance()->Print("PRESS ENTER", 600, 600);

	//プレイヤーがゴールをしたらステージクリア
	if (!isStageClear) {
		if (player->GetIsGoal()) {
			isStageClear = true;
			StageManager::StageClear();
		}
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
	mapData->Update();
	//天球
	skydome->Update();

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	//binary出力
	if (player->GetIsMove() || camera->GetIsTriggerDimensionChange()) {
		orderNum++;
		orderMaxNum = orderNum;
		if (deleteOrderMaxNum < orderMaxNum) {
			deleteOrderMaxNum = orderMaxNum;
		}
		Vector3 cameraPos = camera->GetEye();
		Vector3 playerPos = player->GetPosition();
		XMINT3 mapChip = player->GetMapChipNumberPos();
		JsonLoader::SerializeBinary(std::to_string(orderNum), camera->GetIs2D(), player->GetMoveSurfacePhase(),
			{ mapChip.x,mapChip.y,mapChip.z }, { cameraPos.x,cameraPos.y,cameraPos.z },
			{ camera->GetCameraXPosPhase(),camera->GetCameraYPosPhase() }, { playerPos.x,playerPos.y,playerPos.z });
	}

	//undo
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Z)) {
		if (orderNum != 0) {
			orderNum--;
			bool is2D = false;
			int moveSurface = 0;
			std::array<int, 3> mapChip{};
			std::array<float, 3> cameraPos{}, playerPos{};
			std::array<int, 2> cameraPosPhase{};
			JsonLoader::DeserializeBinary(std::to_string(orderNum), &is2D, &moveSurface, &mapChip, &cameraPos, &cameraPosPhase, &playerPos);

			if (camera->GetIs2D() != is2D) {
				camera->SetIs2D(is2D);
				camera->SetDirtyProjection(true);
			}
			player->SetMoveSurfacePhase(moveSurface);
			player->SetMapChipNumberPos({ mapChip[0],mapChip[1],mapChip[2] });
			camera->SetEye({ cameraPos[0],cameraPos[1],cameraPos[2] });
			camera->SetCameraXPosPhase(cameraPosPhase[0]);
			camera->SetCameraYPosPhase(cameraPosPhase[1]);
			player->SetPosition({ playerPos[0],playerPos[1],playerPos[2] });
		}
	}
	//redo
	else if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Y)) {
		if (orderNum != orderMaxNum) {
			orderNum++;
			bool is2D = false;
			int moveSurface = 0;
			std::array<int, 3> mapChip{};
			std::array<float, 3> cameraPos{}, playerPos{};
			std::array<int, 2> cameraPosPhase{};
			JsonLoader::DeserializeBinary(std::to_string(orderNum), &is2D, &moveSurface, &mapChip, &cameraPos, &cameraPosPhase, &playerPos);

			if (camera->GetIs2D() != is2D) {
				camera->SetIs2D(is2D);
				camera->SetDirtyProjection(true);
			}
			player->SetMoveSurfacePhase(moveSurface);
			player->SetMapChipNumberPos({ mapChip[0],mapChip[1],mapChip[2] });
			camera->SetEye({ cameraPos[0],cameraPos[1],cameraPos[2] });
			camera->SetCameraXPosPhase(cameraPosPhase[0]);
			camera->SetCameraYPosPhase(cameraPosPhase[1]);
			player->SetPosition({ playerPos[0],playerPos[1],playerPos[2] });
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_RETURN) && !isSceneChange) {
		//シーン切り替え
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
		//binary削除
		DeleteBinary();
		//次のステージへ
		StageManager::NextStageSelect();
	}
	//シーン変更状態
	SceneChangeMode();
	//シーン変更演出更新
	SceneChangeEffect::Update();
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::Draw3D()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///

	//プレイヤー
	player->Draw();
	//天球
	skydome->Draw();

	///-------Object3d描画ここまで-------///

	///-------Instance描画ここから-------///

	InstanceObject::DrawPrev();
	//マップ用ブロック
	mapData->Draw();

	///-------Instance描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void TitleScene::Draw3DLightView()
{
}

void TitleScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//シーン変更演出描画
	SceneChangeEffect::Draw();


	///-------スプライト描画ここまで-------///
}
