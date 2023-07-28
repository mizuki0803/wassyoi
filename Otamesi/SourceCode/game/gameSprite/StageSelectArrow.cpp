#include "StageSelectArrow.h"
#include "SpriteTextureLoader.h"
#include "StageManager.h"
#include "WindowApp.h"

StageSelectArrow* StageSelectArrow::Create()
{
	//インスタンス生成
	StageSelectArrow* instance = new StageSelectArrow();

	// 初期化
	instance->Initialize();

	return instance;
}

void StageSelectArrow::Initialize()
{
	//スプライト生成
	const Vector2 centerPos = { WindowApp::window_width / 2, WindowApp::window_height / 2 }; //	中心座標
	const Vector2 positionCenterDistance = { 500, 0 }; //中心からの距離

	//左矢印
	arrows[0].reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::StageSelectArrow), centerPos - positionCenterDistance, { 0.5f, 0.5f }, true));
	//右矢印
	arrows[1].reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::StageSelectArrow), centerPos + positionCenterDistance));

	//描画チェックと色設定
	for (int i = 0; i < 2; i++) {
		arrows[i]->SetColor({ 0.8f, 0, 0, 1 });
		isDraws[i] = true;
	}
	SelectStageCheckDraw();
}

void StageSelectArrow::Update()
{
	//スプライト描画
	for (int i = 0; i < 2; i++) {
		//描画しない状態なら飛ばす
		if (!isDraws[i]) { continue; }
		arrows[i]->Update();
	}
}

void StageSelectArrow::Draw()
{
	//スプライト描画
	for (int i = 0; i < 2; i++) {
		//描画しない状態なら飛ばす
		if (!isDraws[i]) { continue; }
		arrows[i]->Draw();
	}
}

void StageSelectArrow::SelectStageCheckDraw()
{
	//ステージ1を選択中の場合は左矢印の描画を停止
	if (StageManager::GetSelectStage() == 1) {
		isDraws[0] = false;
		return;
	}
	//最後のステージを選択中の場合は右矢印の描画を停止
	else if (StageManager::GetSelectStage() == StageManager::GetStageNum()) {
		isDraws[1] = false;
		return;
	}

	//それ以外なら描画を行う
	for (int i = 0; i < 2; i++) {
		isDraws[i] = true;
	}
}
