#include "SceneChangeEffect.h"

std::list<std::unique_ptr<SceneChangeSprite>> SceneChangeEffect::sceneChangeSprites;

void SceneChangeEffect::Update()
{
	//死亡状態のシーン変更演出用スプライトの削除
	sceneChangeSprites.remove_if([](std::unique_ptr<SceneChangeSprite>& sceneChangeSprite) {
		return sceneChangeSprite->GetIsDead();
		});

	//シーン変更演出用スプライト更新
	for (const std::unique_ptr<SceneChangeSprite>& sceneChangeSprite : sceneChangeSprites) {
		sceneChangeSprite->Update();
	}
}

void SceneChangeEffect::Draw()
{
	//シーン変更演出用スプライト描画
	for (const std::unique_ptr<SceneChangeSprite>& sceneChangeSprite : sceneChangeSprites) {
		sceneChangeSprite->Draw();
	}
}

void SceneChangeEffect::CreateSceneChangeEffect(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime)
{
	//新しいシーン変更演出用スプライトを生成する
	std::unique_ptr<SceneChangeSprite> newSceneChangeSprite;
	newSceneChangeSprite.reset(SceneChangeSprite::Create(color, deepenTime, waitTime, returnTime));
	sceneChangeSprites.push_back(std::move(newSceneChangeSprite));
}
