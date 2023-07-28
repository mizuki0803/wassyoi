#include "StageSelectScene.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "ParticleEmitter.h"
#include "MapBlockData.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include "MapDataStageSelect.h"

void StageSelectScene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01", true));

	//マップ管理生成
	mapDataManager.reset(MapDataStageSelectManager::Create());

	//カメラ初期化
	camera.reset(new Camera());
	camera->Initialize();
	camera->SetEye({ 0, 5, -70 });
	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });


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

	//画面にパーティクルが残ることがあるので全て削除しておく
	ParticleEmitter::GetInstance()->AllDelete();

	//ポストエフェクトのブラーを解除しておく
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//UI関係生成
	userInterface_ = UserInterface::Create(UserInterface::GamePhase::Selection);
}

void StageSelectScene::Finalize()
{
}

void StageSelectScene::Update()
{
	//エスケープキーでメニュー画面
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
	mapDataManager->SetNotMove(userInterface_->GetMenuFlag());
	MenuAction();

	//カメラ更新
	camera->Update();
	lightCamera->Update();

	//ライト更新
	lightGroup->Update();

	//オブジェクト更新
	//マップ用ブロック
	mapDataManager->Update();
	//ステージを変更したときのみ、矢印スプライトを描画するか判定する
	if (mapDataManager->GetIsChangeStage()) {
		userInterface_->GetStageSelectArrow()->SelectStageCheckDraw();
	}
	//天球
	skydome->Update();

	//UIの更新
	userInterface_->Update();

	//スペースキーでステージを確定し、ゲームシーンへ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && !userInterface_->GetMenuFlag()) {
		//se再生
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);

		//シーン切り替え
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
	}

	//シーン変更状態
	SceneChangeMode();
	//シーン変更演出更新
	SceneChangeEffect::Update();
}

void StageSelectScene::DrawBackSprite()
{
}

void StageSelectScene::Draw3D()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///

	//天球
	skydome->Draw();

	///-------Object3d描画ここまで-------///

	///-------Instance描画ここから-------///

	InstanceObject::DrawPrev();
	//マップ用ブロック
	mapDataManager->Draw();

	///-------Instance描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void StageSelectScene::Draw3DLightView()
{
}

void StageSelectScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//ステージ番号
	mapDataManager->DrawUI();

	//UI関係
	userInterface_->Draw();

	//シーン変更演出描画
	SceneChangeEffect::Draw();


	///-------スプライト描画ここまで-------///
}

void StageSelectScene::MenuAction()
{
	//メニューが開いていなければ抜ける
	if (!userInterface_->GetMenuFlag()) { return; }
	//決定のスペースキーを押していなければ抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE))) { return; }

	//スペースキーを押した瞬間に選択されている項目によって挙動を設定
	//タイトルシーンに移動
	if (userInterface_->GetSelectionNumber() == (int)UserInterface::StageSelectSceneItem::SceneChangeTitle) {
		//シーン切り替え
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "TITLE");
		//se再生
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);
	}

	//binary削除
	DeleteBinary();
}

void StageSelectScene::FrameReset()
{
	mapDataManager->FrameReset();
}
