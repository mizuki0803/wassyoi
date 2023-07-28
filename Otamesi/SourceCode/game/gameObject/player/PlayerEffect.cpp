#include "PlayerEffect.h"
#include "Player.h"
#include "Easing.h"

PlayerEffect *PlayerEffect::Create(ObjModel *model,float scale)
{
	//objオブジェクトのインスタンスを生成
	PlayerEffect *object3d = new PlayerEffect();
	if (object3d == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize()) {
		delete object3d;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	object3d->SetModel(model);

	object3d->scale =
		1.0f +
		(object3d->maxScale - object3d->minScale) / 3 * scale;

	return object3d;
}

void PlayerEffect::Update(Player *player)
{
	ScaleUp();
	Vector3 nextPos{ player->GetPosition() };
	Vector3 offset{};
	switch (player->moveSurfacePhase)
	{
	case Player::MoveSurfacePhase::Upward:
		offset = { 0,-2.4f,0 };
		SetRotation({ 0,0,0 });
		break;
	case Player::MoveSurfacePhase::Downward:
		offset = { 0,2.4f,0 };
		SetRotation({ 180,0,0 });
		break;
	case Player::MoveSurfacePhase::FacingLeft:
		offset = { 2.4f,0,0 };
		SetRotation({ 0,0,-90 });
		break;
	case Player::MoveSurfacePhase::FacingRight:
		offset = { -2.4f,0,0 };
		SetRotation({ 0,0,90 });
		break;
	case Player::MoveSurfacePhase::FacingForward:
		offset = { 0,0,2.4f };
		SetRotation({ 90,0,0 });
		break;
	case Player::MoveSurfacePhase::FacingAway:
		offset = { 0,0,-2.4f };
		SetRotation({ -90,0,0 });
		break;
	default:
		break;
	}

	nextPos += offset;
	SetPosition(nextPos);

	ObjObject3d::Update();
}


void PlayerEffect::ScaleUp()
{
	scale += scaleUp;
	
	if (color.w <= 0.0f)
	{
		color.w = 1.0f;
		scale = minScale;
	}
	if (scale >= colorEaseStart)
	{
		color.w = Easing::InQuart(1.0f, 0.0f, (scale - colorEaseStart) / (maxScale - colorEaseStart));
		if (color.w < 0.0f)
		{
			color.w = 0.0f;
		}
	}
	SetScale({ scale,scale ,scale });

}
