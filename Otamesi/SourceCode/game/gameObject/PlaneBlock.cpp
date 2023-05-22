#include "PlaneBlock.h"

PlaneBlock* PlaneBlock::Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& rotation)
{
	//インスタンス生成
	PlaneBlock* instance = new PlaneBlock();

	//初期化処理
	if (!instance->Initialize(model, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//ブロックの種類を「ハリボテ」に設定
	instance->blockType = BlockType::Plane;

	return instance;
}

void PlaneBlock::aaaa()
{
	color = { 1,0,0,1 };
}
