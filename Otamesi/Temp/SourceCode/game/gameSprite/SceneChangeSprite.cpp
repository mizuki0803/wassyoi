#include "SceneChangeSprite.h"
#include "SpriteTextureLoader.h"
#include "WindowApp.h"
#include "Easing.h"

void (SceneChangeSprite::* SceneChangeSprite::intensityChangeActionFuncTable[])() = {
	&SceneChangeSprite::ColorDeepen,
	&SceneChangeSprite::ColorWait,
	&SceneChangeSprite::ColorReturn,
};

SceneChangeSprite* SceneChangeSprite::Create(const XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime)
{
	//シーン変更演出用スプライトのインスタンスを生成
	SceneChangeSprite* sceneChangeSprite = new SceneChangeSprite();
	if (sceneChangeSprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!sceneChangeSprite->Initialize(SpriteTextureLoader::GetTexture(SpriteTextureLoader::White), {0, 0}, false, false)) {
		delete sceneChangeSprite;
		assert(0);
		return nullptr;
	}

	//色をセット
	sceneChangeSprite->color = { color.x, color.y, color.z, 0 };

	//時間をセット
	sceneChangeSprite->deepenTime = deepenTime;
	sceneChangeSprite->waitTime = waitTime;
	sceneChangeSprite->returnTime = returnTime;

	return sceneChangeSprite;
}

bool SceneChangeSprite::Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	//スプライト初期化
	if (!Sprite::Initialize(texture, anchorpoint, isFlipX, isFlipY)) {
		return false;
	}

	//画面全体を覆うのに必要な情報をセット
	position = { 0, 0 };
	size = { WindowApp::window_width, WindowApp::window_height };
	texSize = { 1, 1 };

	return true;
}

void SceneChangeSprite::Update()
{
	//色の濃さ変更挙動
	(this->*intensityChangeActionFuncTable[static_cast<size_t>(phase)])();

	//スプライト更新
	Sprite::Update();
}

void SceneChangeSprite::ColorDeepen()
{
	//タイマー更新
	intensityChangeTimer++;
	//イージング用タイム
	const float time = (float)intensityChangeTimer / deepenTime;

	//だんだん色を濃くしていく
	color.w = Easing::LerpFloat(0, 1, time);

	//タイマーが指定した時間になったら
	if (intensityChangeTimer >= deepenTime) {
		//次のフェーズへ
		phase = IntensityChangePhase::Wait;

		//タイマー初期化
		intensityChangeTimer = 0;
	}
}

void SceneChangeSprite::ColorWait()
{
	//タイマー更新
	intensityChangeTimer++;

	//タイマーが指定した時間になったら
	if (intensityChangeTimer >= waitTime) {
		//次のフェーズへ
		phase = IntensityChangePhase::Return;

		//タイマー初期化
		intensityChangeTimer = 0;
	}
}

void SceneChangeSprite::ColorReturn()
{
	//タイマー更新
	intensityChangeTimer++;
	//イージング用タイム
	const float time = (float)intensityChangeTimer / returnTime;

	//だんだん色を薄くしていく
	color.w = Easing::LerpFloat(1, 0, time);

	//タイマーが指定した時間になったら
	if (intensityChangeTimer >= returnTime) {
		//死亡フラグを立てる
		isDead = true;
	}
}
