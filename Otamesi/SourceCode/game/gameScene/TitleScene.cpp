#include "TitleScene.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "ParticleEmitter.h"
#include "MapBlockData.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"

void TitleScene::Initialize()
{
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	//デバッグ用テキスト
	DebugText::GetInstance()->Print("TITLE SCENE", 350, 200, 5);
	DebugText::GetInstance()->Print("PRESS SPACE", 600, 600);

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//シーン切り替え
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
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
