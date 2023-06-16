#include "GameLightCamera.h"

GameLightCamera* GameLightCamera::Create(float centerDistance)
{
	//インスタンス生成
	GameLightCamera* instance = new GameLightCamera();

	//初期化
	instance->Initialize(centerDistance);

	return instance;
}

void GameLightCamera::Initialize(float centerDistance)
{
	//カメラ初期化
	Camera::Initialize();

	//視点をセット
	up = { 0, 1, 0 };

	//中心からの距離をセット
	this->centerDistance = centerDistance;
}

void GameLightCamera::Update()
{
	//カメラ回転
	Around();

	//カメラ更新
	Camera::Update();
}

void GameLightCamera::Around()
{
	//回転スピード
	const float rotNum = 0.2f;
	//回転角を更新
	aroundRotate += rotNum;

	//中心点からの距離をぐるぐる回る計算
	const float angle = XMConvertToRadians(aroundRotate);
	//アンダーフローする可能性があるので、小数点を切り捨てる
	const float divNum = 1000;
	const float roundAngle = floorf(angle * divNum) / divNum;

	//ラジアンを使用し、sin,cosを算出
	const float sinfAngle = sinf(roundAngle);
	const float cosfAngle = cosf(roundAngle);

	//計算結果を割り当てて視点をセット
	Vector3 aroundPos{};
	aroundPos.x = sinfAngle * centerDistance;
	aroundPos.y = cosfAngle * centerDistance;

	SetEye(aroundPos);
}
