#include "Block.h"

const float Block::blockSize = 5.0f;

Block* Block::Create(ObjModel* model, const XMINT3& mapChipNum)
{
	//インスタンス生成
	Block* instance = new Block();

	//初期化処理
	if (!instance->Initialize(model, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//ブロックの種類を「ブロック」に設定
	instance->blockType = BlockType::Block;

	return instance;
}

bool Block::Initialize(ObjModel* model, const XMINT3& mapChipNum)
{
	//モデルをセット
	assert(model);
	this->model = model;

	//object3Dの初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//座標をセット
	position = { mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() };
	//大きさをセット
	scale = { blockSize, blockSize, blockSize };

	return true;
}
