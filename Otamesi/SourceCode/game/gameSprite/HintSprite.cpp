#include "HintSprite.h"
#include "Easing.h"
#include "WindowApp.h"

const float HintSprite::normalTextureScale = 0.15f;
const float HintSprite::largeTextureScale = 0.7f;


HintSprite* HintSprite::Create(const Texture& texture, DrawerSprite* parent, const Vector2 position, const Vector2& anchorpoint)
{
	//インスタンス生成
	HintSprite* instance = new HintSprite();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化処理
	if (!instance->Initialize(texture, parent, position, anchorpoint)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool HintSprite::Initialize(const Texture& texture, DrawerSprite* parent, const Vector2 position, const Vector2& anchorpoint)
{
	//スプライト生成
	if (!Sprite::Initialize(texture, position, anchorpoint, false, false)) {
		return false;
	}
	this->parent = parent;

	//保存用情報をセット
	parentStorage = parent;
	positionStorage = position;
	anchorpointStorage = anchorpoint;

	//大きさをセット
	SetScale(normalTextureScale);

	return true;
}

void HintSprite::Update()
{
	//大きさ変更
	SizeChange();

	//スプライト更新
	Sprite::Update();
}

void HintSprite::SizeChangeStart()
{
	//大きさを変更中の場合は抜ける
	if (isSizeChange) { return; }

	//大きさが通常の場合は、大きくする
	if (!isSizeLarger) {

		//移動前座標をセット
		sizeChangeBeforePos = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1] };
		//移動後座標をセット
		sizeChangeAfterPos = { WindowApp::window_width / 2, WindowApp::window_height / 2 };
		//親とは別の動きをするので、親子構造を解除
		parent = nullptr;

		//サイズが大きい状態に変更
		isSizeLarger = true;
	}
	//大きさが大きい場合は、元に戻す
	else {
		//親子構造を復活させる
		parent = parentStorage;
		//移動前座標をセット
		sizeChangeBeforePos = position - parent->GetPosition();
		//移動後座標をセット
		sizeChangeAfterPos = positionStorage;

		//サイズが小さい状態に変更
		isSizeLarger = false;
	}

	//大きさを変更状態にする
	isSizeChange = true;
	//タイマーを初期化
	actionTimer = 0;
}

void HintSprite::SizeChange()
{
	//大きさ変更中でなければ抜ける
	if (!isSizeChange) { return; }

	//タイマー更新
	actionTimer++;
	const float moveTime = 30; //座標移動にかかる時間

	//イージングに使用する変数(0～1を算出)
	const float time = actionTimer / moveTime;
	//移動させる
	position.x = Easing::OutQuint(sizeChangeBeforePos.x, sizeChangeAfterPos.x, time);
	position.y = Easing::OutQuint(sizeChangeBeforePos.y, sizeChangeAfterPos.y, time);
	
	float size;	//大きさ変更用変数
	//大きくする場合
	if (isSizeLarger) {
		//大きさを変更
		size = Easing::OutQuint(normalTextureScale, largeTextureScale, time);

		//アンカーポイント変更
		anchorpoint.x = Easing::OutQuint(anchorpointStorage.x, 0.5f, time);
		anchorpoint.y = Easing::OutQuint(anchorpointStorage.y, 0.5f, time);
	}
	//元に戻す場合
	else {
		//大きさを変更
		size = Easing::OutQuint(largeTextureScale, normalTextureScale, time);

		//アンカーポイント変更
		anchorpoint.x = Easing::OutQuint(0.5f, anchorpointStorage.x, time);
		anchorpoint.y = Easing::OutQuint(0.5f, anchorpointStorage.y, time);
	}
	SetScale(size);
	

	//タイマーが指定した時間に満たなければ抜ける
	if (actionTimer < moveTime) { return; }

	//大きさ変更状態を解除
	isSizeChange = false;
}
