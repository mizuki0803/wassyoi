#include "PlaneBlock.h"

PlaneBlock* PlaneBlock::Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& rotation)
{
	//インスタンス生成
	PlaneBlock* instance = new PlaneBlock();

	//モデルをセット
	assert(model);
	instance->model = model;

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	// イージングデータの設定
	instance->SetEaseData(60);

	//座標をセット
	Vector3 temppos = { mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() };
	temppos.y = -100.0f;
	instance->SetBlockStratPos(temppos);
	instance->SetBlockEndPos({ mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() });
	instance->position = temppos;
	//大きさをセット
	instance->scale = { blockSize, blockSize, blockSize };
	//角度をセット
	instance->rotation = rotation;

	// 関数の設定
	instance->CreateAct();

	//ブロックの種類を「はりぼて」に設定
	instance->blockType = BlockType::Plane;

	return instance;
}

void PlaneBlock::aaaa()
{
	color = { 1,0,0,1 };
}
