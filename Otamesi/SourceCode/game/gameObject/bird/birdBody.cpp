#include "birdBody.h"

BirdBody* BirdBody::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
    //インスタンス生成
	BirdBody* instance = new BirdBody();

	//モデルをセット
	assert(model);
	instance->model = model;

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//座標をセット
	instance->position = position;
	//移動速度をセット
	instance->velocity = velocity;

	return instance;
}

void BirdBody::Update()
{
	//飛行移動
	FlyMove();

	//オブジェクト更新
	ObjObject3d::Update();
}

void BirdBody::FlyMove()
{
	//座標に移動速度を加算して移動させる
	position += velocity;

	//移動速度の方向を向くようにする
	rotation = Vector3::VelocityRotate(velocity);
}
