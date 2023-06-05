#include "PlayerEffect.h"
#include "Player.h"


PlayerEffect *PlayerEffect::Create(ObjModel *model,float scale)
{
	//obj�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	PlayerEffect *object3d = new PlayerEffect();
	if (object3d == nullptr) {
		return nullptr;
	}

	// ������
	if (!object3d->Initialize()) {
		delete object3d;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	object3d->SetModel(model);

	object3d->scale = scale;

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
		offset = { 0,-2.5f,0 };
		SetRotation({ 0,0,0 });
		break;
	case Player::MoveSurfacePhase::Downward:
		offset = { 0,2.5f,0 };
		SetRotation({ 180,0,0 });
		break;
	case Player::MoveSurfacePhase::FacingLeft:
		offset = { 2.5f,0,0 };
		SetRotation({ 0,0,-90 });
		break;
	case Player::MoveSurfacePhase::FacingRight:
		offset = { -2.5f,0,0 };
		SetRotation({ 0,0,90 });
		break;
	case Player::MoveSurfacePhase::FacingForward:
		offset = { 0,0,2.5f };
		SetRotation({ 90,0,0 });
		break;
	case Player::MoveSurfacePhase::FacingAway:
		offset = { 0,0,-2.5f };
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
	SetScale({ scale,scale ,scale });
	scale += 0.02f;
	if (scale > maxScale)
	{

		scale = minScale;
	}
}
