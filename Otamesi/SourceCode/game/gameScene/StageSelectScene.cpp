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
}

void StageSelectScene::Finalize()
{
}

void StageSelectScene::Update()
{
	//デバッグ用テキスト
	//DebugText::GetInstance()->Print("STAGESELECT SCENE", 270, 60, 5);
	//DebugText::GetInstance()->Print("PRESS ENTER", 600, 600);


	//カメラ更新
	camera->Update();
	lightCamera->Update();

	//ライト更新
	lightGroup->Update();

	//オブジェクト更新
	//マップ用ブロック
	mapDataManager->Update();
	//天球
	skydome->Update();

	//スペースキーでステージを確定し、ゲームシーンへ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//se再生
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);

		//シーン切り替え
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
	}
	//エスケープキーでタイトルシーンへ
	else if (Input::GetInstance()->PushKey(DIK_ESCAPE)) {
		//シーン切り替え
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "TITLE");
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

	//シーン変更演出描画
	SceneChangeEffect::Draw();


	///-------スプライト描画ここまで-------///
}
