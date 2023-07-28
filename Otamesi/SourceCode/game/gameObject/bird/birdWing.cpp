#include "birdWing.h"

BirdWing* BirdWing::Create(ObjModel* model, ObjObject3d* parent, WingType wingType, const Vector3& position)
{
	//インスタンス生成
	BirdWing* instance = new BirdWing();

	//モデルをセット
	assert(model);
	instance->model = model;

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//親オブジェクトをセット
	instance->parent = parent;
	//翼のタイプをセット
	instance->wingType = wingType;
	//座標をセット
	instance->position = position;

	//左翼なら半回転させる
	if (wingType == LeftWing) {	instance->rotation.z = 180;	}

	return instance;
}

void BirdWing::Update()
{
	//羽ばたき
	Flap();

	//オブジェクト更新
	ObjObject3d::Update();
}

void BirdWing::Flap()
{
	//回転の最大スピード
	const float rotateMaxSpeed = 1.0f;
	//回転の加速度
	const float rotateSpeedAccel = 0.1f;
	//翼の折り返し回転角
	const float wingFoldRotate = 45.0f;

	//翼を上げる場合
	if (isWingUp) {
		//回転スピードに加速度を加算して加速させる
		rotateSpeed += rotateSpeedAccel;
		rotateSpeed = min(rotateSpeed, rotateMaxSpeed);

		if (wingType == WingType::LeftWing) {
			//z軸に回転スピードを加算する
			rotation.z += rotateSpeed;

			//折り返し地点まで到達したら折り返す
			if (rotation.z >= 180 + wingFoldRotate) {
				isWingUp = false;
			}
		}
		else if (wingType == WingType::RightWing) {
			//z軸に回転スピードを加算する
			rotation.z += -rotateSpeed;

			//折り返し地点まで到達したら折り返す
			if (rotation.z <= -wingFoldRotate) {
				isWingUp = false;
			}
		}
	}
	//翼を下げる場合
	else {
		//回転スピードに加速度を加算して加速させる
		rotateSpeed -= rotateSpeedAccel;
		rotateSpeed = max(rotateSpeed, -rotateMaxSpeed);

		if (wingType == WingType::LeftWing) {
			//z軸に回転スピードを加算する
			rotation.z += rotateSpeed;

			//折り返し地点まで到達したら折り返す
			if (rotation.z <= 180 - wingFoldRotate) {
				isWingUp = true;
			}
		}
		else if (wingType == WingType::RightWing) {
			//z軸に回転スピードを加算する
			rotation.z += -rotateSpeed;

			//折り返し地点まで到達したら折り返す
			if (rotation.z >= wingFoldRotate) {
				isWingUp = true;
			}
		}
	}
}
