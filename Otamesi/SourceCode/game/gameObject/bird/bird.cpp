#include "bird.h"
#include "Easing.h"

float Bird::deadLine = 200.0f;

void (Bird::* Bird::lifePhasefuncTable[])() = {
	&Bird::BornAction,
	&Bird::DeadCheck,
	&Bird::FadeoutAction,
};

Bird* Bird::Create(ObjModel* bodyModel, ObjModel* wingModel, const Vector3& position, const Vector3& velocity)
{
	//インスタンス生成
	Bird* instance = new Bird();

	// 初期化
	if (!instance->Initialize(bodyModel, wingModel, position, velocity)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool Bird::Initialize(ObjModel* bodyModel, ObjModel* wingModel, const Vector3& position, const Vector3& velocity)
{
	//胴体生成
	body.reset(BirdBody::Create(bodyModel, position, velocity));

	//両翼生成
	wings[0].reset(BirdWing::Create(wingModel, body.get(), BirdWing::WingType::LeftWing, {}));
	wings[1].reset(BirdWing::Create(wingModel, body.get(), BirdWing::WingType::RightWing, {}));

	return true;
}

void Bird::Update()
{
	//行動
	(this->*lifePhasefuncTable[static_cast<size_t>(lifePhase)])();

	//胴体更新
	body->Update();

	//両翼更新
	for (const std::unique_ptr<BirdWing>& wing : wings) {
		wing->Update();
	}
}

void Bird::Draw()
{
	//胴体描画
	body->Draw();

	//両翼描画
	for (const std::unique_ptr<BirdWing>& wing : wings) {
		wing->Draw();
	}
}

void Bird::BornAction()
{
	//タイマー更新
	actionTimer++;
	const float bornTime = 1000;	//産まれるのにかかる時間
	const float time = actionTimer / bornTime;	//イージング用時間

	//胴体と翼の大きさを大きくする
	const float size = Easing::InQuad(0, 1, time);
	body->SetScale({ size, size, size });

	//タイマーが指定した時間以下なら抜ける
	if (actionTimer < bornTime) { return; }

	//生きている状態にする
	lifePhase = LifePhase::Alive;
	//タイマー初期化
	actionTimer = 0;
}

void Bird::DeadCheck()
{
	//座標を取り出す
	const Vector3 pos = body->GetPosition();
	const float posXYZ[3] = { pos.x, pos.y, pos.z };

	//x,y,zのそれぞれを判定
	for (int i = 0; i < 3; i++) {
		//死亡ラインを超えていなければ飛ばす
		if (posXYZ[i] <= deadLine && posXYZ[i] >= -deadLine) { continue; }

		//消えていく状態にする
		lifePhase = LifePhase::Fadeout;
		//抜ける
		return;
	}
}

void Bird::FadeoutAction()
{
	//タイマー更新
	actionTimer++;
	const float fadeoutTime = 1000;	//フェードアウトにかかる時間
	const float time = actionTimer / fadeoutTime;	//イージング用時間

	//胴体と翼の大きさを小さくする
	const float size = Easing::OutQuad(1, 0, time);
	body->SetScale({size, size, size});

	//タイマーが指定した時間以下なら抜ける
	if (actionTimer < fadeoutTime) { return; }

	//死亡状態にする
	isDead = true;
}
