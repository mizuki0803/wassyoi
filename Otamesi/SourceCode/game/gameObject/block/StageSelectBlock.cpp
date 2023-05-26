#include "StageSelectBlock.h"

StageSelectBlock* StageSelectBlock::Create(const int _blockType, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent)
{
	//インスタンス生成
	StageSelectBlock* instance = new StageSelectBlock();

	//初期化処理
	if (!instance->Initialize(_blockType, mapChipNum, mapSize, parent)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageSelectBlock::Initialize(const int _blockType, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent)
{
	//ブロックの種類を「ブロック」に設定
	blockType = BROCK_TYPE(_blockType);

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
