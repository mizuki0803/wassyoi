#include "StageSelectBlock.h"

StageSelectBlock* StageSelectBlock::Create(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos, ObjObject3d* parent)
{
	//インスタンス生成
	StageSelectBlock* instance = new StageSelectBlock();

	//初期化処理
	if (!instance->Initialize(_blockType, mapChipNum, shiftPos, parent)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageSelectBlock::Initialize(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos, ObjObject3d* parent)
{
	//ブロックの種類を「ブロック」に設定
	blockType = BROCK_TYPE(_blockType);

	//座標をセット
	Vector3 pos = { mapChipNum.x * blockSize, mapChipNum.y * blockSize, mapChipNum.z * blockSize };
	pos -= shiftPos;
	position = pos;
	//大きさをセット
	scale = { blockSize, blockSize, blockSize };
	//親オブジェクトをセット
	this->parent = parent;

	return true;
}
