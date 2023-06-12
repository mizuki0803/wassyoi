#include "StageClearUI.h"
#include "SpriteTextureLoader.h"
#include "WindowApp.h"

StageClearUI* StageClearUI::Create()
{
	//インスタンス生成
	StageClearUI* instance = new StageClearUI();

	//初期化処理
	instance->Initialize();

	return instance;
}

void StageClearUI::Initialize()
{
	//UI生成
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::StageClear), { WindowApp::window_width / 2, 300 }, { 513, 84 }, 2.0f);	//ステージクリア
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::PressSpace), { WindowApp::window_width / 2, 660 }, { 550, 85 }, 0.5f);	//PRESS SPACE
}

void StageClearUI::Update()
{
	//スプライト更新
	for (const std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Update();
	}
}

void StageClearUI::Draw()
{
	//スプライト描画
	for (const std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Draw();
	}
}

void StageClearUI::CreateNewSprite(const Texture& texture, const Vector2& position, const Vector2& texSize, const float sizeRatio)
{
	//スプライト生成
	std::unique_ptr<Sprite> newSprite;
	newSprite.reset(Sprite::Create(texture));
	newSprite->SetPosition(position);
	newSprite->SetTexSize(texSize);
	newSprite->SetSize(texSize * sizeRatio);
	sprites.push_back(std::move(newSprite));
}
