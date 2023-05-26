#include "StageSelectBlock.h"

StageSelectBlock* StageSelectBlock::Create(ObjModel* model, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent)
{
	//インスタンス生成
	StageSelectBlock* instance = new StageSelectBlock();

	//初期化処理
	if (!instance->Initialize(model, mapChipNum, mapSize, parent)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//ブロックの種類を「ブロック」に設定
	instance->blockType = BlockType::Block;

	return instance;
}

bool StageSelectBlock::Initialize(ObjModel* model, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent)
{
	//モデルをセット
	assert(model);
	this->model = model;

	//object3Dの初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//座標をセット
	Vector3 pos = { mapChipNum.x * blockSize, mapChipNum.y * blockSize, mapChipNum.z * blockSize };
	pos -= { blockSize / 2 * (mapSize.x - 1), blockSize / 2 * (mapSize.y - 1), blockSize / 2 * (mapSize.z - 1) };
	position = pos;
	//大きさをセット
	scale = { blockSize, blockSize, blockSize };
	//親オブジェクトをセット
	this->parent = parent;

	return true;
}
