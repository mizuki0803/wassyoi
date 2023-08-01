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
#include "HintTextureLoader.h"
#include <cassert>
#include <fstream>
#include <iomanip>
#include "OutLine.h"


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
	const float lightCameraCenterDistance = 80;
	lightCamera.reset(GameLightCamera::Create(lightCameraCenterDistance));
	lightCamera->SetProjectionNum({ 250, 250 }, { -250, -250 });

	OutLine::SetCmaera(camera.get());

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

	//UI関係生成
	HintTextureLoader::LoadTextures(StageManager::GetSelectStage());
	userInterface_ = UserInterface::Create(UserInterface::GamePhase::Game);
	//ステージクリアUI生成
	stageClear_ = ClearStaging::Create();


}

void GameScene::Finalize()
{
	DeleteBinary();
}

void GameScene::Update()
{
	ObjObject3d::SetCamera(camera.get());

	if (!isStageClear) {
		//全てのステージをクリア後の特別なステージのみスペースキー入力でタイトルシーンへ
		if (StageManager::GetSelectStage() == 100) {
			if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
				//シーン切り替え
				SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "TITLE");
			}
		}
		//その他ステージは通常の動き
		else {
			//次元変更が可能かUIに伝える
			userInterface_->IsChangeDimensionCheck(player->ChangeDimensionStartCheck());

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
			//エスケープキーでメニュー画面
			else if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
				if (!userInterface_->GetMenuFlag())
				{
					userInterface_->SetMenuFlag(true);
				}
				else
				{
					userInterface_->SetMenuFlag(false);
				}
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
		mapData->SetIsAllStageClear(StageManager::GetIsAllStageClear());
		mapData->SetSelectStageNum(StageManager::GetSelectStage());

		if (StageManager::GetIsAllStageClear() && StageManager::GetSelectStage() >= 100)
		{
			ParticleEmitter::GetInstance()->DemoShine(Vector3(0.0f, 0.0f, -(mapData->GetCameraDist().z - 20.0f)), 30.0f, 1);
		}
	}
	else {
		if (stageClear_->GetIntermediateTrigger())
		{
			//binary削除
			DeleteBinary();
			//初めて全てのステージをクリアした場合は特別なステージへ
			if (StageManager::AllStageClearTriggerCheck()) {
				//カメラの回転を元に戻す状態にする
				camera->SetClearResetAround();
			}
			//次のステージがある場合は次のステージへ
			else if (StageManager::NextStageSelect()) {
				//カメラの回転を元に戻す状態にする
				camera->SetClearResetAround();
			}
			//次のステージがない場合はステージセレクトシーン
			else {
				//シーン切り替え
				SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "STAGESELECT");
			}
		}
		else if (stageClear_->GetEndFlag())
		{
			//再生成
			ReCreate();
			stageClear_->Reset();
		}

		//マップが再生成を終えたら、次のステージを開始するためにフラグなどをリセット
		if (mapData->GetIsReCreateEnd()) {
			RestartGame();
		}
	}

	camera->SetNotMove(userInterface_->GetMenuFlag(), mapData->GetIsMoveEnd(), userInterface_->GetIsHintViewMode());
	player->SetNotMove(userInterface_->GetMenuFlag(), mapData->GetIsMoveEnd(), userInterface_->GetIsHintViewMode());
	userInterface_->SetNotMove(isStageClear);
	MenuAction();

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
	//背景オブジェクト
	backGround->Update();

	//スプライト
	//UIの更新
	userInterface_->Update();
	//ステージクリアUI更新
	stageClear_->Update();

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

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
}

void GameScene::Draw3D()
{

	//Object3d共通コマンド
	ObjObject3d::DrawOutLinePrev();

	///-------Object3d描画ここから-------///
	// �A�E�g���C���p�萔�o�b�t�@�Z�b�g
	GamePostEffect::SetIdColorBuffer(5,PostEffect::kNone);
	//天球
	skydome->Draw();
	////�v���C���[
	GamePostEffect::SetIdColorBuffer(5,PostEffect::kPlayer);
	//プレイヤー
	player->Draw();

	///-------Object3d描画ここまで-------///

	///-------Instance描画ここから-------///

	//マップ用ブロック
	InstanceObject::DrawPrev(InstanceObject::PipelineType::OutLine);
	mapData->Draw();



	//背景オブジェクト
	InstanceObject::DrawPrev();
	backGround->Draw();


	///-------Instance描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void GameScene::AfterBloomDraw()
{
	player->EffectDraw();
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

	//UI関係
	userInterface_->Draw();

	//ステージクリア
	stageClear_->Draw();

	//シーン変更演出描画
	SceneChangeEffect::Draw();
	
	///-------スプライト描画ここまで-------///
}

void GameScene::DrawImageForUI()
{
	userInterface_->DrawMenuImage();
}

void GameScene::MenuAction()
{
	//メニューが開いていなければ抜ける
	if (!userInterface_->GetMenuFlag()) { return; }
	//決定のスペースキーを押していなければ抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE))) { return; }

	//スペースキーを押した瞬間に選択されている項目によって挙動を設定
	//ステージ選択シーンへの移行
	if (userInterface_->GetSelectionNumber() == (int)UserInterface::GameSceneItem::SceneChangeStageSelect) {
		//シーン切り替え
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "STAGESELECT");
		//se再生
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);
	}
	else if (userInterface_->GetSelectionNumber() == (int)UserInterface::GameSceneItem::SceneChangeTitle) {
		//シーン切り替え
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "TITLE");
		//se再生
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);
	}

	//binary削除
	DeleteBinary();
}

void GameScene::ReCreate()
{
	mapData->ReCreateMapBlock(StageManager::GetSelectStage());
	player->ReCreate(mapData->GetPlayerCreateMapChipNum(), mapData->GetShiftPos());
	player->SetMoveSurfacePhase(mapData->GetInstallationSurface());
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());
	camera->SetReCreateMove(mapData->GetCameraDist());
	skydome->SetIsRotate(true);
}

void GameScene::RestartGame()
{
	//次のステージを開始するためにフラグなどをリセット
	isStageClear = false;
	mapData->SetIsReCreateEnd(false);
	skydome->SetIsRotate(false);
	userInterface_->SetMenuFlag(false);
	HintTextureLoader::LoadTextures(StageManager::GetSelectStage());
	userInterface_->StageChangeUpdate();
	//次元変更が可能かUIに伝える
	userInterface_->IsChangeDimensionCheck(player->ChangeDimensionStartCheck());

	//全てのステージをクリア後の特別なステージのみその他の設定をリセットさせずに抜ける
	if (StageManager::GetSelectStage() == 100) { return; }

	//次のステージを開始するためにフラグなどをリセット
	player->Reset();
	camera->Reset();
	orderMaxNum = orderNum = deleteOrderMaxNum = 0;
	KeepBinary(*camera, *player);
}

void GameScene::FrameReset()
{
	mapData->FrameReset();
	backGround->FrameReset();
}
