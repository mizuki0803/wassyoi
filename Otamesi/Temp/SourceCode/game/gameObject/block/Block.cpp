#include "Block.h"

const float Block::blockSize = 5.0f;
std::unique_ptr<BaseBlock> Block::instanceBlock;

void Block::StaticInitialize()
{
	instanceBlock = std::make_unique<BaseBlock>();
	instanceBlock->StaticInitialize();
}

Block* Block::Create(const int _blockType, const XMINT3& mapChipNum)
{
	//インスタンス生成
	Block* instance = new Block();

	//初期化処理
	if (!instance->Initialize(_blockType, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

void Block::Draw()
{
	instanceBlock->Draw();
}

bool Block::Initialize(const int _blockType, const XMINT3& mapChipNum)
{
	//ブロックの種類を「ブロック」に設定
	blockType = BROCK_TYPE(_blockType);

	//座標をセット
	position = { mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() };
	//大きさをセット
	scale = { blockSize, blockSize, blockSize };

	return true;
}

void Block::Update()
{
	if (parent != nullptr) {
		instanceBlock->Update(int(blockType), { position.x,position.y,position.z }, scale, rotation, color, &parent->GetMatWorld());
	} else {
		instanceBlock->Update(int(blockType), { position.x,position.y,position.z }, scale, rotation, color);
	}
}
