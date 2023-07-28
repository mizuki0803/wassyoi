#include "BirdManager.h"

BirdManager* BirdManager::Create(ObjModel* bodyModel, ObjModel* wingModel, const int32_t& createInterval)
{
	//インスタンス生成
	BirdManager* instance = new BirdManager();

	//モデルをセット
	assert(bodyModel);
	instance->bodyModel = bodyModel;
	assert(wingModel);
	instance->wingModel = wingModel;

	//生成間隔をセット
	instance->createInterval = createInterval;

	return instance;
}

void BirdManager::Update()
{
	//死亡状態の鳥を削除
	birds.remove_if([](std::unique_ptr<Bird>& bird) {
		return bird->GetIsDead();
		});

	//生成タイマー更新
	createTimer++;
	//鳥生成
	CreateBird();

	//鳥更新
	for (const std::unique_ptr<Bird>& bird : birds) {
		bird->Update();
	}
}

void BirdManager::Draw()
{
	//鳥描画
	for (const std::unique_ptr<Bird>& bird : birds) {
		bird->Draw();
	}
}

void BirdManager::CreateBird()
{
	//タイマーが生成間隔まで到達していなければ抜ける
	if (createTimer < createInterval) { return; }
	
	//新たな鳥のために座標と移動速度を設定していく
	Vector3 pos;
	Vector3 vel;

	//ステージの前後左右どこを飛ぶのかを設定
	const FlyPosition flyPosition = static_cast<FlyPosition>((rand() % PosNum));
	//+方向と-方向どちらに飛行か設定
	const bool isFlyDirectionPlus = static_cast<bool>(rand() % 2);

	//飛行方向に合わせて座標をセット
	const float stageCenterDistance = 100.0f; //ステージ中央からの距離
	if (flyPosition == FlyPosition::Forward) { pos.z = -stageCenterDistance; }
	else if (flyPosition == FlyPosition::Away) { pos.z = stageCenterDistance; }
	else if (flyPosition == FlyPosition::Left) { pos.x = -stageCenterDistance; }
	else if (flyPosition == FlyPosition::Right) { pos.x = stageCenterDistance; }	

	//開始地点を設定
	float createLine = 200;	//生成時の飛行開始ライン
	//+方向に飛行する場合はマイナスにする
	if (isFlyDirectionPlus) { createLine *= -1; }

	//飛行方向に合わせて速度を設定
	float flySpeed = 0.2f;
	//-方向に飛行する場合はマイナスにする
	if (!isFlyDirectionPlus) { flySpeed *= -1; }

	//設定した開始地点と飛行速度をセット
	if (flyPosition == FlyPosition::Forward || flyPosition == FlyPosition::Away) { pos.x = createLine; vel.x = flySpeed; }
	else if (flyPosition == FlyPosition::Left || flyPosition == FlyPosition::Right) { pos.z = createLine; vel.z = flySpeed; }

	//Y軸の高さをランダムで設定
	const float posYRandRange = 50.0f;	//ランダムの範囲
	const float posYBase = 20.0f;	//最小値
	pos.y = (float)rand() / RAND_MAX * posYRandRange + posYBase;

	//新たな鳥を生成
	std::unique_ptr<Bird> newBird;
	newBird.reset(Bird::Create(bodyModel, wingModel, pos, vel));
	birds.push_back(std::move(newBird));

	//タイマーをリセット
	createTimer = 0;
}
