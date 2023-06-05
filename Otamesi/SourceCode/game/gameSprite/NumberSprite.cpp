#include "NumberSprite.h"

NumberSprite* NumberSprite::Create(const Texture& texture, const Vector2& position, const Vector2& size, const Vector2& texSize)
{
	//数字スプライトのインスタンスを生成
	NumberSprite* numberSprite = new NumberSprite();
	if (numberSprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!numberSprite->Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		delete numberSprite;
		assert(0);
		return nullptr;
	}

	//座標をセット
	numberSprite->position = position;

	//大きさをセット
	numberSprite->size = size;
	numberSprite->texSize = texSize;

	return numberSprite;
}

void NumberSprite::SetNumber(const int number)
{
	//スプライトの左上切り出し座標をテクスチャサイズ * 数字で算出する
	texLeftTop.x = texSize.x * number;
}
