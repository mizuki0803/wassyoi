#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "ParticleEmitter.h"
#include "PlayerActionManager.h"
#include "MapBlockData.h"
#include "JsonLoader.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include "StageManager.h"
#include "SpriteTextureLoader.h"
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
	modelPlayerEffect.reset(ObjModel::LoadFromOBJ("effect"));
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01", true));

	//マップ生成
	mapData.reset(MapDataStage::Create(StageManager::GetSelectStage()));

	//背景オブジェクト生成
	backGround.reset(BackGround::Create());

	//カメラ初期化
	camera.reset(new GameCamera());
	const XMFLOAT3 distanceStageCenter = mapData->GetCameraDist(); //カメラ視点のステージ中央からの距離
	const XMINT3 mapSize = mapData->GetMapSize(); //マップの大きさ
	const Vector3 stageCenterPos = {}; //ステージ中央座標
	camera->Initialize(distanceStageCenter, stageCenterPos);
	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });

	//プレイヤー生成
	player.reset(Player::Create(modelPlayer.get(), mapData->GetPlayerCreateMapChipNum(), mapData->GetShiftPos(), camera.get(), modelPlayerEffect.get()));
	player->SetMoveSurfacePhase(mapData->GetInstallationSurface());
	//プレイヤーの移動可能判定用にマップ番号をセット
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());
	//カメラに生成したプレイヤーをセット
	camera->SetPlayer(player.get());

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));

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
	KeepBinary(*camera, *player);

	userInterface_ = UserInterface::Create();
	//操作方法UI生成
	howToPlayUI.reset(HowToPlayUI::Create(true));
	//ステージクリアUI生成
	stageClearUI.reset(StageClearUI::Create());
	stageClear_ = ClearStaging::Create();

	// スカイドーム生成
	//paranomaSkyDorm.reset(dynamic_cast<ParanomaSkyDorm*>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::ParanomaSky))));
}

void GameScene::Finalize()
{
	DeleteBinary();
}

void GameScene::Update()
{
	if (!isStageClear) {
		//undo
		if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Z)) {
			Undo(camera.get(), player.get());
		}
		//redo
		else if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Y)) {
			Redo(camera.get(), player.get());
		}

		//Rキーでリセット
		if (Input::GetInstance()->TriggerKey(DIK_R)) {
			//シーン切り替え
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
			//binary削除
			DeleteBinary();
		}
		//エスケープキーでメニュー画面(今は即ステージセレクトへ)
		else if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
			//se再生
			Audio::GetInstance()->PlayWave(Audio::SoundName::button);
			//シーン切り替え
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "STAGESELECT");
			//binary削除
			DeleteBinary();
		}

		//binary出力
		if (player->GetIsMove() || (!player->GetNowMove() && camera->GetIsTriggerDimensionChange())) {
			orderNum++;
			orderMaxNum = orderNum;
			if (deleteOrderMaxNum < orderMaxNum) {
				deleteOrderMaxNum = orderMaxNum;
			}

			KeepBinary(*camera, *player);
		}

		//プレイヤーがゴールをしたらステージクリア
		if (player->GetIsGoal()) {
			//クリア音
			Audio::GetInstance()->PlayWave(Audio::SoundName::clear);

			isStageClear = true;
			StageManager::StageClear();
			camera->SetClearMode();
			stageClear_->SetMovePhase(ClearStaging::MovePhase::Start);
		}
	}
	else {
		if (stageClear_->GetEndFlag())
		{
			//binary削除
			DeleteBinary();
			//次のステージへ
			StageManager::NextStageSelect();
			//再生成
			ReCreate();
			stageClear_->Reset();

		}
		//マップが再生成を終えたら、次のステージを開始するためにフラグなどをリセット
		if (mapData->GetIsReCreateEnd()) {
			RestartGame();
		}
	}

	//カメラ更新
	camera->Update();
	lightCamera->Update();

	//ライト更新
	lightGroup->Update();

	// スカイドーム更新
	//paranomaSkyDorm->Update();

	//オブジェクト更新
	//プレイヤー
	player->Update();
	//マップ用ブロック
	mapData->Update();
	//天球
	skydome->Update();
	//背景オブジェクト
	backGround->Update();

	//スプライト
	//UIの更新
	userInterface_->Update();
	//操作方法
	howToPlayUI->Update();
	//ステージクリアUI更新
	stageClearUI->Update();
	stageClear_->Update();

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	if (Input::GetInstance()->TriggerKey(DIK_M))
	{
		if (!userInterface_->GetMenuFlag())
		{
			userInterface_->SetMenuFlag(true);
		}
		else
		{
			userInterface_->SetMenuFlag(false);
		}
	}

	//初期状態をbinary保存
	if (player->GetIsStartMove()) {
		KeepBinary(*camera, *player);
	}

	//シーン変更状態
	SceneChangeMode();
	//シーン変更演出更新
	SceneChangeEffect::Update();
}

void GameScene::DrawBackSprite()
{
	/*SpriteCommon::GetInstance()->DrawPrev("SkyDorm");
	paranomaSkyDorm->Draw();*/
}

void GameScene::Draw3D()
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
	//背景オブジェクト
	backGround->Draw();

	///-------Instance描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void GameScene::Draw3DLightView()
{
	///-------Instance描画ここから-------///

	InstanceObject::DrawLightViewPrev();

	//背景オブジェクト
	backGround->DrawLightCameraView();

	///-------Instance描画ここまで-------///
}

void GameScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//操作方法
	howToPlayUI->Draw();

	userInterface_->Draw();

	//ステージクリアUI
	/*if (isStageClear) {
		stageClearUI->Draw();
	}*/

	stageClear_->Draw();

	//シーン変更演出描画
	SceneChangeEffect::Draw();


	///-------スプライト描画ここまで-------///
}

void GameScene::ReCreate()
{
	mapData->ReCreateMapBlock(StageManager::GetSelectStage());
	player->ReCreate(mapData->GetPlayerCreateMapChipNum(), mapData->GetShiftPos());
	player->SetMoveSurfacePhase(mapData->GetInstallationSurface());
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());
	camera->SetReCreateMove();
	camera->SetSaveDistanceStageCenter(mapData->GetCameraDist());
	skydome->SetIsRotate(true);
}

void GameScene::RestartGame()
{
	//次のステージを開始するためにフラグなどをリセット
	isStageClear = false;
	mapData->SetIsReCreateEnd(false);
	player->Reset();
	camera->Reset();
	skydome->SetIsRotate(false);
}
