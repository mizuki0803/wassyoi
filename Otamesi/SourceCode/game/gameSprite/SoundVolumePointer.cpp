#include "SoundVolumePointer.h"

SoundVolumePointer* SoundVolumePointer::Create(const Texture& texture, const Vector2 barLeftPos, float barLength, float startPercentage)
{
	//インスタンス生成
	SoundVolumePointer* instance = new SoundVolumePointer();

	//初期化
	if (!instance->Initialize(texture, barLeftPos, barLength, startPercentage)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool SoundVolumePointer::Initialize(const Texture& texture, const Vector2 barLeftPos, float barLength, float startPercentage)
{
	//スプライト生成
	if (!Sprite::Initialize(texture, barLeftPos, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//バーの左端座標をセット
	this->barLeftPos = barLeftPos;
	//バーの長さをセット
	this->barLength = barLength;

	//生成時のパーセンテージに合わせて座標をセット
	SetPercentage(startPercentage);

	return true;
}

void SoundVolumePointer::SetPercentage(float percentage)
{
	//割合に応じて座標を変更
	position.x = barLeftPos.x + (barLength * percentage);
}
