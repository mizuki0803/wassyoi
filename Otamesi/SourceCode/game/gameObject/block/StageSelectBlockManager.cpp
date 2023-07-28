#include "StageSelectBlockManager.h"
#include "Easing.h"

std::array<Vector3, StageSelectBlockManager::PositionNum> StageSelectBlockManager::managerPos = {
	{{0, 0, 0}, {150, 0, 0}, {-150, 0, 0}}
};

StageSelectBlockManager* StageSelectBlockManager::Create(const BlockManagerPositionPhase& positionPhase)
{
	//インスタンス生成
	StageSelectBlockManager* instance = new StageSelectBlockManager();

	//初期化処理
	if (!instance->Initialize(positionPhase)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageSelectBlockManager::Initialize(const BlockManagerPositionPhase& positionPhase)
{
	//オブジェクト生成
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//座標をセット
	position = managerPos[positionPhase];

	//現在の場所フェーズをセット
	blockManagerPositionPhase = positionPhase;

	//場所フェーズが中心以外なら
	if (!positionPhase == Center) {
		//中心に移動させる
		MoveStart(Center);
	}

	return true;
}

void StageSelectBlockManager::Update()
{
	//くるくる回転させる
	const float rotSpeed = 0.5f;
	rotation.y += rotSpeed;

	//移動
	Move();

	//オブジェクト更新
	ObjObject3d::Update();
}

void StageSelectBlockManager::MoveStart(const BlockManagerPositionPhase& nextPositionPhase)
{
	//既に移動中なら抜ける
	if (isMove) { return; }

	//次に移動する場所フェーズをセット
	this->nextBlockManagerPositionPhase = nextPositionPhase;

	//移動用タイマーを初期化しておく
	moveTimer = 0;
	//移動中に設定する
	isMove = true;
}

void StageSelectBlockManager::Move()
{
	//移動状態でないなら抜ける
	if (!isMove) { return; }

	//移動用タイマーを更新していく
	moveTimer++;
	//イージング用数値を計算する
	const float time = moveTimer / (float)moveTime;

	//座標を更新
	position.x = Easing::OutQuad(managerPos[blockManagerPositionPhase].x, managerPos[nextBlockManagerPositionPhase].x, time);

	//タイマーが指定した時間以下なら抜ける
	if (moveTimer < moveTime) { return; }

	//現在の場所フェーズ(移動前)が中心であれば削除フラグを立てる
	if (blockManagerPositionPhase == BlockManagerPositionPhase::Center) {
		isDelete = true;
	}
	//場所フェーズが中心以外であれば
	else {
		//移動後用に設定していた場所フェーズを現在の場所フェーズに設定
		blockManagerPositionPhase = nextBlockManagerPositionPhase;
	}

	//移動状態を解除しておく
	isMove = false;
}
