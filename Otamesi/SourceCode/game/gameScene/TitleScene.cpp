#include "TitleScene.h"
#include "WindowApp.h"
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
#include "SpriteTextureLoader.h"
#include "OutLine.h"

void TitleScene::Initialize()
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

	//選択中のステージ番号を0にセット
	StageManager::SetSelectStage(0);
	//マップ生成
	mapData.reset(MapDataStage::Create(StageManager::GetSelectStage()));

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

	//背景オブジェクト生成
	backGround.reset(BackGround::Create());

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

	//UI関係生成
	userInterface_ = UserInterface::Create(UserInterface::GamePhase::Title);
	//タイトルロゴ生成
	titleLogo.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::TitleLogo)));
	titleLogo->SetPosition({ WindowApp::window_width / 2, 140 });
	titleLogo->SetTexSize({1658, 518});
	titleLogo->SetSize(titleLogo->GetTexSize() * 0.4f);
}

void TitleScene::Finalize()
{
	DeleteBinary();
}

void TitleScene::Update()
{
	//デバッグ用テキスト
	//DebugText::GetInstance()->Print("TITLE SCENE", 350, 200, 5);

	if (!isStageClear) {
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
			camera->SetIsStageClear(true);

			//シーン切り替え
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
			//binary削除
			DeleteBinary();
			//次のステージへ
			StageManager::NextStageSelect();
		}

		//エスケープキーでメニュー
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))
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
	}

	camera->SetNotMove(userInterface_->GetMenuFlag(), mapData->GetIsMoveEnd());
	player->SetNotMove(userInterface_->GetMenuFlag(), mapData->GetIsMoveEnd());
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

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	//タイトルロゴ
	titleLogo->Update();

	//初期状態をbinary保存
	if (player->GetIsStartMove()) {
		KeepBinary(*camera, *player);
	}

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

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
	ObjObject3d::DrawOutLinePrev();
	///-------Object3d描画ここから-------///
	// アウトライン用定数バッファセット
	GamePostEffect::SetIdColorBuffer(5, PostEffect::kNone);
	//天球
	skydome->Draw();
	////プレイヤー
	GamePostEffect::SetIdColorBuffer(5, PostEffect::kPlayer);
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

void TitleScene::AfterBloomDraw()
{
	player->EffectDraw();
}

void TitleScene::Draw3DLightView()
{
	///-------Instance描画ここから-------///

	InstanceObject::DrawLightViewPrev();

	//背景オブジェクト
	backGround->DrawLightCameraView();

	///-------Instance描画ここまで-------///
}

void TitleScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//UI関係
	userInterface_->Draw();

	//タイトルロゴ
	titleLogo->Draw();

	//シーン変更演出
	SceneChangeEffect::Draw();


	///-------スプライト描画ここまで-------///
}

void TitleScene::MenuAction()
{
	//メニューが開いていなければ抜ける
	if (!userInterface_->GetMenuFlag()) { return; }
	//決定のスペースキーを押していなければ抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE))) { return; }

	//スペースキーを押した瞬間に選択されている項目によって挙動を設定
	//.exeの終了
	if (userInterface_->GetSelectionNumber() == (int)UserInterface::TitleSceneItem::ExeEnd) {
		isEndRequest = true;
		//se再生
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);
	}

	//binary削除
	DeleteBinary();
}

void TitleScene::FrameReset()
{
	mapData->FrameReset();
	backGround->FrameReset();
}
