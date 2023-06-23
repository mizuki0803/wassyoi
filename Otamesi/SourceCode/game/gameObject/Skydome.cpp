#include "Skydome.h"

const float Skydome::size = 10.0f;
const float Skydome::rotSpeed = 4.3f;

Skydome* Skydome::Create(ObjModel* model)
{
	//インスタンスを生成
	Skydome* instance = new Skydome();

	//モデルをセット
	assert(model);
	instance->model = model;

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//大きさをセット
	instance->scale = { size, size, size };

	return instance;
}

void Skydome::Update()
{
	//回転
	Rotate();

	//オブジェクト更新
	ObjObject3d::Update();
}

void Skydome::Rotate()
{
	//回転状態でなければ抜ける
	if (!isRotate) { return; }

	rotation.y += -rotSpeed;
}
