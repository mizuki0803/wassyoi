#include "GamePostEffect.h"

std::unique_ptr<PostEffect> GamePostEffect::postEffect;

void GamePostEffect::Initialize()
{
	//ゲームで使用するポストエフェクト生成
	postEffect.reset(PostEffect::Create());
}

void GamePostEffect::Draw()
{
	//ポストエフェクト描画
	postEffect->Draw();
}

void GamePostEffect::DrawScenePrev()
{
	//ポストエフェクトシーン描画前処理
	postEffect->DrawScenePrev();
}

void GamePostEffect::DrawSceneRear()
{
	//ポストエフェクトシーン描画後処理
	postEffect->DrawSceneRear();
}
