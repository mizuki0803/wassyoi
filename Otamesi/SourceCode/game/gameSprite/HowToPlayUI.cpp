#include "HowToPlayUI.h"
#include "SpriteTextureLoader.h"

HowToPlayUI* HowToPlayUI::Create(bool isDrawPause)
{
	//インスタンス生成
	HowToPlayUI* instance = new HowToPlayUI();

	//初期化処理
	instance->Initialize(isDrawPause);

	return instance;
}

void HowToPlayUI::Initialize(bool isDrawPause)
{
	//UI生成
	Vector2 leftTopPos = { 20, 20 };
	const float sizeRatio = 0.3f;	//テクスチャサイズに対する大きさの比率
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), leftTopPos, { 690, 110 }, sizeRatio);		//移動
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), leftTopPos, { 800, 110 }, sizeRatio);	//カメラ

	//ポーズの描画をしないなら抜ける
	if (!isDrawPause) { return; }
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayPause), leftTopPos, { 533, 110 }, sizeRatio);		//ポーズ
}

void HowToPlayUI::Update()
{
	//スプライト更新
	for (const std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Update();
	}
}

void HowToPlayUI::Draw()
{
	//スプライト描画
	for (const std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Draw();
	}
}

void HowToPlayUI::CreateNewSprite(const Texture& texture, Vector2& position, const Vector2& texSize, const float sizeRatio)
{
	//スプライト生成
	std::unique_ptr<Sprite> newSprite;
	newSprite.reset(Sprite::Create(texture, { 0, 0 }));
	newSprite->SetPosition(position);
	newSprite->SetTexSize(texSize);
	newSprite->SetSize(texSize * sizeRatio);
	newSprite->SetColor({ 1, 1, 0, 1 });
	sprites.push_back(std::move(newSprite));

	//次のスプライトの左上座標をセットしておく
	position.y += texSize.y * sizeRatio;
}
